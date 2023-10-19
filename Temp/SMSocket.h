// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "SMSocket.generated.h"

USTRUCT(BlueprintType)
struct FTrackerData
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
		FVector location;
	UPROPERTY(BlueprintReadWrite)
		FRotator rotation;
};

struct SendAndRecvData
{
public:
	/*float distance;
	float angle;*/
	uint32 TimeTag;
	FTrackerData trackerData;
	uint8 CommandData;
};

struct FReadBuff
{
	//读取缓存区
	uint8* ReadBuffer;
	uint32 ReadBufferLen;
	uint32 FrameSize;
	uint32 BufferMAX;
	uint8* TempBuf;
};


class WaitForClinent :public FNonAbandonableTask
{
public:
	WaitForClinent(TFunction<void(FSocket*)> InFunctionA, FSocket* InServer);
	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(DeiveSocketTask, STATGROUP_ThreadPoolAsyncTasks); };
	void DoWork();
private:
	FSocket* ServerSocket;
	TFunction<void(FSocket*)> BackUPClient;
};


UCLASS()
class TRACKERSERVER_API ASMSocket : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASMSocket();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	//创建初始化sokcet
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool InitializeSocket(const int& ServerPort);
	//关闭Socket
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool CloseSocket();
	//打开线程监听客户端是否连接成功
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool OpenMonitorConnection();
	//发送数据到客户端
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool SendDataToClient(FTrackerData tData, int32 DataCmd);
	//清理客户端缓存
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		void ClearClientData();
	//测试模式发送数据
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool DeBugSend(float distance, float angle, int32 DataTimeTag, int32 DataCmd, int32 MyTags);
	//接收读取客户端数据
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool RecvClientData();
	//数据接收回调
	UFUNCTION(BlueprintImplementableEvent, Category = "SM|Socket")
		void  OnReturnClientCmd(int32 CmdTag);
	//获取客户端的地址
	UFUNCTION(BlueprintCallable, Category = "SM|Socket", BlueprintPure = true)
		FString GetClientAddr();
	//获取当前客户端指针是否为空
	UFUNCTION(BlueprintCallable, Category = "SM|Socket", BlueprintPure = true)
		bool GetClientIsConnection();
	//动态实时获取当前的ip地址是否有效
	UFUNCTION(BlueprintCallable, Category = "SM|Socket", BlueprintPure = true)
		FString GetServerAddress();
	//关闭相应的task任务
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		void CloseTask();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "SM|Socket")
		FString AddrStr;
	UPROPERTY(BlueprintReadOnly, Category = "SM|Socket")
		FString SendDataHex;
	UPROPERTY(BlueprintReadOnly, Category = "SM|Socket")
		FString ReadDataHex;
private:
	//将数据转化为传缓冲区
	uint8* DataToBuffer(FTrackerData tData, int32 DataCmd);
	//读取缓冲区数据
	void ReadBuferData();
	//将缓冲区数据转换为可视化数据
	void BufferToData(const uint8* InBuffer);

private:
	//服务器socket
	FSocket* ServerSocket;
	//客户端Socket
	FSocket* ClientSocket;
	//返回客户端指针跟新
	TFunction<void(FSocket*)> SetClientSocket;
	//等客户端连接线程
	FAutoDeleteAsyncTask<WaitForClinent>* WaitClientTask;
	//缓存区长度
	uint32 BufferSize;
	//缓存区针头
	uint8 BufferHead;
	//缓存区针尾
	uint8 BufferEnd;
	//缓存区
	uint8* SendBuffer;
	//读取缓存区
	FReadBuff ReadBuffer;


};
