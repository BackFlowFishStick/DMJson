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
	//��ȡ������
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
	//������ʼ��sokcet
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool InitializeSocket(const int& ServerPort);
	//�ر�Socket
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool CloseSocket();
	//���̼߳����ͻ����Ƿ����ӳɹ�
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool OpenMonitorConnection();
	//�������ݵ��ͻ���
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool SendDataToClient(FTrackerData tData, int32 DataCmd);
	//����ͻ��˻���
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		void ClearClientData();
	//����ģʽ��������
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool DeBugSend(float distance, float angle, int32 DataTimeTag, int32 DataCmd, int32 MyTags);
	//���ն�ȡ�ͻ�������
	UFUNCTION(BlueprintCallable, Category = "SM|Socket")
		bool RecvClientData();
	//���ݽ��ջص�
	UFUNCTION(BlueprintImplementableEvent, Category = "SM|Socket")
		void  OnReturnClientCmd(int32 CmdTag);
	//��ȡ�ͻ��˵ĵ�ַ
	UFUNCTION(BlueprintCallable, Category = "SM|Socket", BlueprintPure = true)
		FString GetClientAddr();
	//��ȡ��ǰ�ͻ���ָ���Ƿ�Ϊ��
	UFUNCTION(BlueprintCallable, Category = "SM|Socket", BlueprintPure = true)
		bool GetClientIsConnection();
	//��̬ʵʱ��ȡ��ǰ��ip��ַ�Ƿ���Ч
	UFUNCTION(BlueprintCallable, Category = "SM|Socket", BlueprintPure = true)
		FString GetServerAddress();
	//�ر���Ӧ��task����
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
	//������ת��Ϊ��������
	uint8* DataToBuffer(FTrackerData tData, int32 DataCmd);
	//��ȡ����������
	void ReadBuferData();
	//������������ת��Ϊ���ӻ�����
	void BufferToData(const uint8* InBuffer);

private:
	//������socket
	FSocket* ServerSocket;
	//�ͻ���Socket
	FSocket* ClientSocket;
	//���ؿͻ���ָ�����
	TFunction<void(FSocket*)> SetClientSocket;
	//�ȿͻ��������߳�
	FAutoDeleteAsyncTask<WaitForClinent>* WaitClientTask;
	//����������
	uint32 BufferSize;
	//��������ͷ
	uint8 BufferHead;
	//��������β
	uint8 BufferEnd;
	//������
	uint8* SendBuffer;
	//��ȡ������
	FReadBuff ReadBuffer;


};
