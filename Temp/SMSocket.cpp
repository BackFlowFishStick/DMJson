// Fill out your copyright notice in the Description page of Project Settings.
#include "SMSocket.h"
#include <iostream>
#include "SocketSubsystem.h"
#include "Runtime/Networking/Public/Networking.h"


ASMSocket::ASMSocket() :
	ServerSocket(NULL),
	ClientSocket(NULL),
	WaitClientTask(NULL),
	BufferHead(0xEE),
	BufferEnd(0xEF),
	SendBuffer(NULL)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	SetClientSocket = [this](FSocket* Client) {ClientSocket = Client; WaitClientTask = NULL; };
	BufferSize = (((sizeof(SendAndRecvData) + 2) * 2));
	SendBuffer = (uint8*)malloc(BufferSize);
	ReadBuffer.ReadBuffer = (uint8*)malloc(((sizeof(SendAndRecvData) + (sizeof(uint8) * 2)) * 11));
	ReadBuffer.BufferMAX = ((sizeof(SendAndRecvData) + (sizeof(uint8) * 2)) * 10);
	ReadBuffer.FrameSize = ((sizeof(SendAndRecvData) + 2));
	ReadBuffer.ReadBufferLen = 0;
	ReadBuffer.TempBuf = (uint8*)malloc(((sizeof(SendAndRecvData) + (sizeof(uint8) * 2)) * 11));
}

void ASMSocket::BeginPlay() 
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("The size of SendAndRecvData: %d"), sizeof(SendAndRecvData)));
}

void ASMSocket::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	Super::EndPlay(EndPlayReason);
	CloseSocket();
	free(SendBuffer);
	free(ReadBuffer.ReadBuffer);
	free(ReadBuffer.TempBuf);
}

void ASMSocket::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

bool ASMSocket::InitializeSocket(const int& ServerPort) 
{
	if (ServerSocket == NULL)
	{
		ServerSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, "Send Information", false);
	}
	bool CanBind = false;
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, CanBind);
	AddrStr = InternetAddr->ToString(false);
	if (CanBind)
	{
		InternetAddr->SetPort(ServerPort);
		if (ServerSocket->Bind(*InternetAddr))
		{
			return true;
		}
	}
	return false;
}

bool ASMSocket::CloseSocket() 
{
	if (ServerSocket) {
		ServerSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ServerSocket);
		ServerSocket = NULL;
	}
	if (ClientSocket) {
		ClientSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
		ClientSocket = NULL;
	}
	return true;
}

bool ASMSocket::OpenMonitorConnection() 
{
	if (ServerSocket != NULL && WaitClientTask == NULL)
	{
		//UE_LOG(LogTemp, Error, TEXT("AServerSocket::OpenMonitorConnection()::OpenTask"));
		ServerSocket->Listen(1);
		WaitClientTask = new FAutoDeleteAsyncTask<WaitForClinent>(SetClientSocket, ServerSocket);
		WaitClientTask->StartBackgroundTask();
		return true;

	}
	return false;
}

bool ASMSocket::SendDataToClient(FTrackerData tData, int32 DataCmd)
{
	if (ClientSocket)
	{
		int32 SendSize = 0;
		if (ClientSocket->Send(DataToBuffer(tData, DataCmd), BufferSize, SendSize))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("The size of SendSize: %d"), sizeof(SendSize)));
			return true;
		}
	}
	return false;
}

void ASMSocket::ClearClientData() 
{
	if (ClientSocket)
	{
		ClientSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
		ClientSocket = NULL;
	}
}

bool ASMSocket::DeBugSend(float distance, float angle, int32 DataTimeTag, int32 DataCmd, int32 MyTags)
{
	return true;
}

bool ASMSocket::RecvClientData() 
{
	uint32 DataSize = {};
	if (ClientSocket)
	{
		if (ClientSocket->HasPendingData(DataSize))
		{
			//UE_LOG(LogTemp, Warning, TEXT("AServerSocket::RecvClientData():: GetPendingData()   = %d"), DataSize);
			//UE_LOG(LogTemp, Warning, TEXT("AReadDevice::RecvClientData() Byte:%zd"), DataSize);
			if (ReadBuffer.ReadBufferLen + DataSize > ReadBuffer.BufferMAX)
			{
				ReadBuferData();
				return true;
			}
			else
			{
				int32 bytesRead = 0;
				if (ClientSocket->Recv((ReadBuffer.ReadBuffer + ReadBuffer.ReadBufferLen), DataSize, bytesRead))
				{
					ReadBuffer.ReadBufferLen += bytesRead;
					//UE_LOG(LogTemp, Warning, TEXT("AReadDevice::RecvClientData() Byte:%s"), DataSize);
					if (this->ReadBuffer.ReadBufferLen >= this->ReadBuffer.FrameSize)
					{
						this->ReadBuferData();
					}
					return true;
				}

			}
		}
	}
	return false;
}

FString ASMSocket::GetClientAddr() 
{
	if (ClientSocket)
	{
		TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); ;
		ClientSocket->GetPeerAddress(*InternetAddr);
		return InternetAddr->ToString(false);
	}
	return "";
}

bool ASMSocket::GetClientIsConnection() 
{
	if (ClientSocket)
	{
		return true;
	}
	return false;
}

FString ASMSocket::GetServerAddress() 
{
	if (ServerSocket)
	{
		bool CanBind = false;
		TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, CanBind);
		return 		AddrStr = InternetAddr->ToString(false);

	}
	return "";
}

void ASMSocket::CloseTask() 
{
	delete WaitClientTask;
}

uint8* ASMSocket::DataToBuffer(FTrackerData tData, int32 DataCmd)
{
	SendAndRecvData DataValue = {};
	/*DataValue.distance = distance;
	DataValue.angle = angle;*/
	DataValue.trackerData = tData;
	DataValue.TimeTag = (uint32)FDateTime::Now().ToUnixTimestamp();
	DataValue.CommandData = (uint32)DataCmd;
	*SendBuffer = BufferHead;
	memcpy(((void*)(SendBuffer + sizeof(uint8))), &DataValue, sizeof(SendAndRecvData));
	*(SendBuffer + (sizeof(BufferHead) + sizeof(SendAndRecvData))) = BufferEnd;
	SendDataHex = BytesToHex(SendBuffer, BufferSize);
	return SendBuffer;
}

void ASMSocket::ReadBuferData() 
{
	uint8 byte = 0;
	int8 headIdx = -1;
	int8 tailIdx = -1;
	for (uint8 i = 0; i < this->ReadBuffer.ReadBufferLen; ++i)
	{
		byte = this->ReadBuffer.ReadBuffer[i];
		//UE_LOG(LogTemp, Log, TEXT("SMSocketTest::ParseRecvData() Byte:%X"), byte);
		//find head
		if (headIdx < 0 && byte == this->BufferHead)
		{
			headIdx = i;
			tailIdx = headIdx + this->ReadBuffer.FrameSize - 1;
			if ((uint32)tailIdx < this->ReadBuffer.ReadBufferLen)
			{
				//frame in buffer
				uint8 tailByte = this->ReadBuffer.ReadBuffer[i + this->ReadBuffer.FrameSize - 1];
				if (tailByte != this->BufferEnd)
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
		if (headIdx >= 0 && byte == this->BufferEnd && i == tailIdx) {
			//find a frame
			this->BufferToData(this->ReadBuffer.ReadBuffer + headIdx);
			headIdx = -1;
			tailIdx = -1;
		}
	}
	if (headIdx < 0 && tailIdx < 0)
	{
		this->ReadBuffer.ReadBufferLen = 0;
	}
	if (headIdx > 0)
	{
		size_t len = this->ReadBuffer.ReadBufferLen - headIdx;
		memcpy(this->ReadBuffer.TempBuf, (this->ReadBuffer.ReadBuffer + headIdx), len);
		memcpy(this->ReadBuffer.ReadBuffer, this->ReadBuffer.TempBuf, len);
		this->ReadBuffer.ReadBufferLen = len;
	}
}

void ASMSocket::BufferToData(const uint8* InBuffer) 
{
	SendAndRecvData RecvData = {};
	RecvData = *((SendAndRecvData*)(InBuffer + 1));
	OnReturnClientCmd(((int32)(RecvData.CommandData)));
}

/*
Ïß³Ì================================================================================================
*/


WaitForClinent::WaitForClinent(TFunction<void(FSocket*)> InFunctionA, FSocket* InServer)
{
	ServerSocket = InServer;
	BackUPClient = InFunctionA;
}


void WaitForClinent::DoWork()
{
	FSocket* Client = ServerSocket->Accept("ClientA");
	BackUPClient(Client);
}
