// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/ASerialReceiverActor.h"
#include "ASerialLibControllerWin.h"
#include "WindowsSerial/WindowsSerial.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ASerialCore/ASerialPacket.h"

class FDeviceCommunicationTask : public FRunnable
{
public:
	FDeviceCommunicationTask(UASerialLibControllerWin* InController, TQueue<FSerialData, EQueueMode::Spsc>* InDataQueue)
		: SerialController(InController), DataQueue(InDataQueue), bIsRunning(true)
	{
	}

	// スレッドのメイン処理
	virtual uint32 Run() override
	{
		// 取得したいコマンドのリスト
		const TArray<uint8> CommandsToProcess = { 0x20,0x21,0x22,0x23,0x24,0x25,0x26 };

		for (int32 i = 0; i < CommandsToProcess.Num(); ++i)
		{
			// スレッドの停止要求があれば、ループを抜ける
			if (!bIsRunning) break;

			const uint8 Command = CommandsToProcess[i];

			// コマンドを送信し、応答を待つ
			if (SerialController->WriteData(Command) == 0)
			{
				ASerialDataStruct::ASerialData ReceivedData;
				if (SerialController->ReadData(&ReceivedData) == 1)
				{
					// 成功したら結果をメインスレッドに渡す
					FSerialData DataToQueue;
					DataToQueue.command = ReceivedData.command;
					DataToQueue.data.Append(ReceivedData.data, ReceivedData.data_num);
					DataQueue->Enqueue(DataToQueue);
				}
			}

			// 最後のコマンドでなければ、2秒間待機する
			if (bIsRunning && i < CommandsToProcess.Num() - 1)
			{
				FPlatformProcess::Sleep(2.0f);
			}
		}

		// 全ての処理が終わったら、スレッドは自動的に終了する
		return 0;
	}

	virtual void Stop() override
	{
		bIsRunning = false;
	}

private:
	UASerialLibControllerWin* SerialController;
	TQueue<FSerialData, EQueueMode::Spsc>* DataQueue;
	FThreadSafeBool bIsRunning;
};

// Sets default values
AASerialReceiverActor::AASerialReceiverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASerialReceiverActor::BeginPlay()
{
	Super::BeginPlay();
	
	SerialController = NewObject<UASerialLibControllerWin>(this);
	SerialInterface = new WindowsSerial();
	SerialController->Initialize(0x04, 0x01);
	SerialController->SetInterfacePt(SerialInterface);

	if (SerialController->AutoConnectDevice() == ConnectResult::Succ)
	{
		bIsDeviceConnected = true;
		UE_LOG(LogTemp, Log, TEXT("Device connected successfully."));

		FPlatformProcess::Sleep(2.0f); // デバイス起動待ち
		//SerialController->WriteData(0x30); // LED点滅
		FPlatformProcess::Sleep(2.0f); // LEDコマンド処理待ち

		// 通信スレッドを開始
		CommunicationTask = new FDeviceCommunicationTask(SerialController, &DataQueue);
		CommunicationThread = FRunnableThread::Create(CommunicationTask, TEXT("DeviceCommunicationThread"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to auto-connect to device."));
	}
}

// Called every frame
void AASerialReceiverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDeviceConnected) return;

	// スレッドから受信したデータを処理
	FSerialData Data;
	while (DataQueue.Dequeue(Data))
	{
		ProcessReceivedData(Data);
	}
}

void AASerialReceiverActor::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	// スレッドを安全に停止・破棄
	if (CommunicationTask)
	{
		CommunicationTask->Stop();
		if (CommunicationThread)
		{
			CommunicationThread->WaitForCompletion();
			delete CommunicationThread;
			CommunicationThread = nullptr;
		}
		delete CommunicationTask;
		CommunicationTask = nullptr;
	}

	if (SerialController && bIsDeviceConnected)
	{
		SerialController->DisConnectDevice();
	}

	if (SerialInterface)
	{
		delete SerialInterface;
		SerialInterface = nullptr;
	}

	Super::EndPlay(EndPlayReason);

}

void AASerialReceiverActor::ProcessReceivedData(const FSerialData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("SUCCESS: Data received for command 0x%X, length: %d"), Data.command, Data.data.Num());

	switch (Data.command)
	{
	case 0x20:

		break;
	case 0x21:

		break;
	case 0x22:

		break;
	case 0x23:

		break;
	case 0x24:

		break;
	case 0x25:

		break;
	case 0x26:

		break;
	}
}
