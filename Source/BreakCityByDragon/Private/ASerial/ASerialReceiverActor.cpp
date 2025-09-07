// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/ASerialReceiverActor.h"
#include "ASerialLibControllerWin.h"
#include "WindowsSerial/WindowsSerial.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ASerialCore/ASerialPacket.h"

class FDeviceComandTask {

private:
	int32_t EulerCmd = 0x21;

public:
	
	static void GetSerialCalibration(UASerialLibControllerWin* Device_) {

		Device_->WriteData(0x26);
		ASerialDataStruct::ASerialData ReceiveData;
		int Result = Device_->ReadData(&ReceiveData);

		// ƒƒO
		uint16_t Error = Device_->GetLastErrorCode();
		UE_LOG(LogTemp, Log, TEXT("Error  : %X"), Error);
		UE_LOG(LogTemp, Log, TEXT("Contact  : %d"), Result);
		UE_LOG(LogTemp, Log, TEXT("Result  ; %x"), ReceiveData.data);
	}

	static void GetEulerAtSensor1(UASerialLibControllerWin* Device_){

		Device_->WriteData(0x20);
		ASerialDataStruct::ASerialData ReceiveData;
		int Result = Device_->ReadData(&ReceiveData);

		// ƒƒO
		uint16_t Error = Device_->GetLastErrorCode();
		UE_LOG(LogTemp, Log, TEXT("Error  : %X"), Error);
		UE_LOG(LogTemp, Log, TEXT("Contact  : %d"), Result);
		UE_LOG(LogTemp, Log, TEXT("Result  ; %x"), ReceiveData.data);
	}
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
		IsDeviceConnected = true;
		UE_LOG(LogTemp, Log, TEXT("Device connected successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to auto-connect to device."));
	}

	FPlatformProcess::Sleep(2.0f);
	FDeviceComandTask::GetSerialCalibration(SerialController);
}

// Called every frame
void AASerialReceiverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeviceCnt += DeltaTime;

	if (!IsDeviceConnected) return;

	// Šp“x‚ðŽæ“¾‚·‚é
	if (DeviceCnt >= MaxDeviceCnt) {
	
		DeviceCnt = 0;
		FDeviceComandTask::GetEulerAtSensor1(SerialController);
	}
}

void AASerialReceiverActor::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	if (SerialController && IsDeviceConnected)
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
