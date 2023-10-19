// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "GameFramework/Actor.h"
#include "OmmoSocket.generated.h"

struct FOmmoRawData 
{
	float location_x_0;
	
	float location_y_0;
	
	float location_z_0;
	
	float rotation_x_0;
	
	float rotation_y_0;
	
	float rotation_z_0;
	
	float rotation_w_0;

	float location_x_1;
	
	float location_y_1;
	
	float location_z_1;

	float rotation_x_1;

	float rotation_y_1;

	float rotation_z_1;

	float rotation_w_1;
};

USTRUCT(BlueprintType)
struct FOmmoTrackerData 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		FVector location;
	UPROPERTY(BlueprintReadWrite)
		FRotator rotation;
};

USTRUCT(BlueprintType)
struct FOmmoData 
{
	GENERATED_BODY()
public:
	/*UPROPERTY(BlueprintReadWrite)
		FOmmoTrackerData tracker_0;
	UPROPERTY(BlueprintReadWrite)
		FOmmoTrackerData tracker_1;*/
	UPROPERTY(BlueprintReadWrite)
		FTransform tracker_0;
	UPROPERTY(BlueprintReadWrite)
		FTransform tracker_1;
};


UCLASS()
class TRACKERSERVER_API AOmmoSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOmmoSocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "SM|OmmoSocket")
		bool InitializeOmmo();
	UFUNCTION(BlueprintCallable, Category = "SM|OmmoSocket")
		bool CloseOmmoSocket();
	UFUNCTION(BlueprintImplementableEvent, Category = "SM|OmmoSocket")
		void OnConnectionFailed();

	UFUNCTION(BlueprintImplementableEvent, Category = "SM|OmmoSocket")
		void OnRecieveOmmoData(FOmmoData trackerData);
	UFUNCTION(BlueprintPure, Category = "SM|OmmoSocket")
		bool GetConnectionStatus();

private:
	void ParseRecvData();
	void ParseFrame(const uint8* Frame);

private:
	FSocket* ommoSocket;

	uint32 RecvBufMax;
	uint32 RecvBufLen;
	uint8* RecvBuf;
	uint8* TempBuf;
	uint32 FrameSize;
	uint8 FrameHead;
	uint8 FrameTail;

	bool isConnectionReady;
	uint32 PrevTimeStamp;

	const uint8 ConnectionWaitTime = 3;

};
