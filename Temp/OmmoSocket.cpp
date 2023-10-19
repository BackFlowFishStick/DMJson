// Fill out your copyright notice in the Description page of Project Settings.


#include "OmmoSocket.h"
#include "SocketSubsystem.h"
#include "Runtime/Networking/Public/Networking.h"

// Sets default values
AOmmoSocket::AOmmoSocket() :
	RecvBufMax(290),
	RecvBufLen(0),
	RecvBuf(NULL),
	TempBuf(NULL),
	FrameSize(58),
	FrameHead(0xFF),
	FrameTail(0xEE),
	ommoSocket(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void AOmmoSocket::BeginPlay()
{
	Super::BeginPlay();
	/*GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("OmmoData size is : %d"), sizeof(FOmmoRawData)));*/
}

// Called every frame
void AOmmoSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->ommoSocket) 
	{
		uint32 Size = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, TEXT("OmmoSocket::Tick() socket is existing"));
		if(this->ommoSocket->HasPendingData(Size)) 
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, TEXT("OmmoSocket::Tick() Pending Data"));
			int32 bytesRead = 0;
			this->ommoSocket->Recv(this->RecvBuf + this->RecvBufLen, Size, bytesRead);
			this->RecvBufLen += bytesRead;
			this->PrevTimeStamp = (uint32)FDateTime::Now().ToUnixTimestamp();
			if (this->RecvBufLen >= this->FrameSize) 
			{
				this->isConnectionReady = true;
				this->ParseRecvData();
			}
		}
	}
	else 
	{
		uint32 disconnectedTime = (uint32)FDateTime::Now().ToUnixTimestamp() - this->PrevTimeStamp;
		if (disconnectedTime > ConnectionWaitTime) 
		{
			//GEngine->AddOnScreenDebugMessage(-1, 45.f, FColor::Red, TEXT("Disconnected"));
			this->isConnectionReady = false;
			OnConnectionFailed();
		}
	}


}

bool AOmmoSocket::InitializeOmmo() 
{
	if (ommoSocket == NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, TEXT("OmmoSocket::InitializeOmmo() Socket Starts"));
		this->ommoSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, "OmmoClient", false);
	}

	if (!this->ommoSocket) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 45.f, FColor::Red, TEXT("OmmoSocket has not been generated"));
		return false;
	}
	
	int32 NewSize = 0;
	this->ommoSocket->SetReceiveBufferSize((int32)this->RecvBufMax, NewSize);
	this->ommoSocket->SetNoDelay(true);
	this->ommoSocket->SetNonBlocking(true);

	bool bCanBindAll = false;
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

	if (bCanBindAll) 
	{
		
		addr->SetPort(2048);
		
		FString _localIp = addr->ToString(true);

		//GEngine->AddOnScreenDebugMessage(-1, 45.f, FColor::Green, FString::Printf(TEXT("The Local IP address: %s"), *_localIp));
		
		if (!this->ommoSocket->Connect(*addr)) 
		{
			//GEngine->AddOnScreenDebugMessage(-1, 45.0f, FColor::Red, TEXT("Could not connect to the IP and/or Port"));
			return false;
		}

		this->RecvBuf = (uint8*)malloc(this->RecvBufMax);
		this->TempBuf = (uint8*)malloc(this->RecvBufMax);
		this->PrevTimeStamp = (uint32)FDateTime::Now().ToUnixTimestamp();

		if (this->RecvBuf == nullptr || this->TempBuf == nullptr) 
		{
			//GEngine->AddOnScreenDebugMessage(-1, 45.f, FColor::Red, TEXT("Memory Allocation Failed"));
			return false;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 45.f, FColor::Green, TEXT("The Socket Starts successfully"));
		return true;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 45.f, FColor::Red, TEXT("The Port cannot be binded"));
	return false;
}

bool AOmmoSocket::CloseOmmoSocket() 
{
	if (this->ommoSocket) 
	{
		this->ommoSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(this->ommoSocket);
		this->ommoSocket = NULL;
		free(this->RecvBuf);
		free(this->TempBuf);
		this->RecvBuf = NULL;
		this->RecvBuf = NULL;
		this->RecvBufLen = 0;
		return true;
	}
	return false;
}

bool AOmmoSocket::GetConnectionStatus() 
{
	return this->isConnectionReady;
}

void AOmmoSocket::ParseRecvData() 
{
	UE_LOG(LogTemp, Log, TEXT("OmmoSocket::ParseRecvData() buf len:%u"), this->RecvBufLen);
	uint8 byte = 0;
	int8 headIdx = -1;
	int8 tailIdx = -1;

	for (uint8 i = 0; i < this->RecvBufLen; ++i) {
		byte = this->RecvBuf[i];
		//find head
		if (headIdx < 0 && byte == this->FrameHead) 
		{
			headIdx = i;
			tailIdx = headIdx + this->FrameSize - 1;
			if ((uint32)tailIdx < this->RecvBufLen) 
			{
				//frame in buffer
				uint8 tailByte = this->RecvBuf[i + this->FrameSize - 1];
				if (tailByte != this->FrameTail) 
				{
					//wrong frame data!
					headIdx = -1;
					tailIdx = -1;
				}
			}
			else 
			{
				//frame part in buffer
				tailIdx = -1;
			}

		}
		//find tail
		if (headIdx >= 0 && byte == this->FrameTail && i == tailIdx) 
		{
			//find a frame
			UE_LOG(LogTemp, Log, TEXT("OmmoSocket::ParseRecvData() find frame:%d-%d|%d"), headIdx, tailIdx, this->RecvBufLen);
			this->ParseFrame(this->RecvBuf + headIdx);
			headIdx = -1;
			tailIdx = -1;
		}

	}

	if (headIdx < 0 && tailIdx < 0) { this->RecvBufLen = 0; }
	if (headIdx > 0) 
	{
		size_t len = this->RecvBufLen - headIdx;
		UE_LOG(LogTemp, Log, TEXT("OmmoSocket::ParseRecvData() copymem len:%zd"), len);
		memcpy(this->TempBuf, this->RecvBuf + headIdx, len);
		memcpy(this->RecvBuf, this->TempBuf, len);
		this->RecvBufLen = len;
	}
}

void AOmmoSocket::ParseFrame(const uint8* Frame) 
{
	FOmmoRawData ommoRawData = *(FOmmoRawData*)(Frame + sizeof(uint8));
	//使用原始数据形成的四元数并将其转为FRotator，将围绕Z轴和x轴的旋转进行镜像后，得到在UnrealEngine中的左手坐标系下的旋转数据
	FQuat rotQuat_0(ommoRawData.rotation_x_0, ommoRawData.rotation_y_0, ommoRawData.rotation_z_0, ommoRawData.rotation_w_0);
	FQuat rotQuat_1(ommoRawData.rotation_x_1, ommoRawData.rotation_y_1, ommoRawData.rotation_z_1, ommoRawData.rotation_w_1);
	FRotator trackerRotation_0 = rotQuat_0.Rotator();
	trackerRotation_0 = FRotator(trackerRotation_0.Pitch, -trackerRotation_0.Yaw, -trackerRotation_0.Roll);
	FRotator trackerRotation_1 = rotQuat_1.Rotator();
	trackerRotation_1 = FRotator(trackerRotation_1.Pitch, -trackerRotation_1.Yaw, -trackerRotation_1.Roll);
	//对位置信息进行左手坐标系转换：以Z轴为轴向旋转180度，并将X轴向位置信息进行镜像，完成转换。
	FVector trackerLocation_0(ommoRawData.location_x_0, ommoRawData.location_y_0, ommoRawData.location_z_0);
	trackerLocation_0 = trackerLocation_0.RotateAngleAxis(180.0f, FVector(0.f, 0.f, 1.0f));
	trackerLocation_0.X = -trackerLocation_0.X;
	FVector trackerLocation_1(ommoRawData.location_x_1, ommoRawData.location_y_1, ommoRawData.location_z_1);
	trackerLocation_1 = trackerLocation_1.RotateAngleAxis(180.0f, FVector(0.f, 0.f, 1.0f));
	trackerLocation_1.X = -trackerLocation_1.X;
	
	FTransform _tracker_0_Transform = FTransform(trackerRotation_0, trackerLocation_0, FVector(1, 1, 1));
	FTransform _tracker_1_Transform = FTransform(trackerRotation_1, trackerLocation_1, FVector(1, 1, 1));

	FOmmoData _data;
	_data.tracker_0 = _tracker_0_Transform;
	_data.tracker_1 = _tracker_1_Transform;

	this->OnRecieveOmmoData(_data);
}



