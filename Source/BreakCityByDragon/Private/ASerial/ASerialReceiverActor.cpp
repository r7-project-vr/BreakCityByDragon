// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/ASerialReceiverActor.h"
#include "ASerialLibControllerWin.h"
#include "WindowsSerial/WindowsSerial.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ASerialCore/ASerialPacket.h"

class FDeviceComandTask {

private:

	UASerialLibControllerWin* Device;

	FRotator I_uintToint(uint8_t* g_) {

		FRotator rotaion = FRotator::ZeroRotator;

		for (int i = 0; i < 3; i++) {

			int32_t num = 0;

			for (int n = 0; n < 4; n++) {

				num += g_[i * 4 + n] << (8 * (3 - n));
			}

			float f = 0.f;

			switch (i)
			{
			case 0:
				f = num / 1000.f;
				rotaion.Roll = f;
				break;
			case 1:
				f = num / 1000.f;
				rotaion.Pitch = f;
				break;
			case 2:
				 f = num / 1000.f;
				rotaion.Yaw = f;
				break;
			default:
				break;
			}
		}
		
		return rotaion;
	}

public:

	FDeviceComandTask(UASerialLibControllerWin* d_) {

		Device = d_;
	}
	
	void GetSerialCalibration() {

		//Device->WriteData(0x00);

		//// ログ
		//uint16_t Error1 = Device->GetLastErrorCode();
		//UE_LOG(LogTemp, Log, TEXT("Error  : %X"), Error1);

		//UE_LOG(LogTemp, Log, TEXT("Device Reset"));


		Device->WriteData(0x26);
		ASerialDataStruct::ASerialData ReceiveData;

		int Result = Device->ReadData(&ReceiveData);

		// ログ
		uint16_t Error = Device->GetLastErrorCode();
		UE_LOG(LogTemp, Log, TEXT("Error  : %X"), Error);
		UE_LOG(LogTemp, Log, TEXT("Contact  : %d"), Result);
		UE_LOG(LogTemp, Log, TEXT("Result  ; %x"), ReceiveData.data[0]);
	}

	FRotator GetSeneserRotation(int senserNum) {

		FRotator rotation = FRotator::ZeroRotator;

		if (senserNum < 1 || senserNum > 4) { 

			UE_LOG(LogTemp, Error, TEXT("index is Non"))
			return rotation;
		}

		uint8_t index[3] = {
			0x23,
			0x24,
			0x25
		};

		Device->WriteData(index[senserNum - 1]);
		ASerialDataStruct::ASerialData ReceiveData;

		int Result = Device->ReadData(&ReceiveData);

		rotation = I_uintToint(ReceiveData.data);

		return rotation;
	}
};

// Sets default values
AASerialReceiverActor::AASerialReceiverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 3; i++) {

		DeviceRotate[i] = FRotator::ZeroRotator;
	}
	
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

	DCT = new FDeviceComandTask(SerialController);
}

// Called every frame
void AASerialReceiverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeviceCnt += DeltaTime;

	// 接続できてなかったら処理なし
	if (!IsDeviceConnected) return;

	// キャリブレーション中は角度の処理をしない
	if (IsCalibration == true) {

		if (DeviceCnt >= MaxCalibrationTime) {

			DeviceCnt = 0;
			IsCalibration = false;
			DCT -> GetSerialCalibration();
			UE_LOG(LogTemp, Log, TEXT("Calibration End"));
		}

		return;
	}

	// 角度を取得する
	if (DeviceCnt >= MaxDeviceCnt) {
	
		DeviceCnt = 0;

		for (int n = 1; n <= 3; n++) {

			DeviceRotate[n - 1] = DCT->GetSeneserRotation(n);
		}
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

FRotator AASerialReceiverActor::GetRotation(int s_) {

	if (s_ < 1 || s_ > 4) {

		return FRotator::ZeroRotator;
	}

	return DeviceRotate[s_ - 1];
}

void AASerialReceiverActor::GetDeviceRotate(FRotator* r) {

	int size = sizeof(r) / sizeof((r)[0]);
	int RotateSize= sizeof(DeviceRotate) / sizeof(DeviceRotate[0]);

	if (size != RotateSize) { return; }

	for (int n = 0; n < RotateSize; n++)
		*(r) = DeviceRotate[n];
}