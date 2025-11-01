// Fill out your copyright notice in the Description page of Project Settings.

#include "ASerial/ASerialReceiverActor.h"
#include "ASerialLibControllerWin.h"
#include "WindowsSerial/WindowsSerial.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ASerialCore/ASerialPacket.h"
#include "ASerial/RawDataCalculator.h"
#include "Kismet/KismetSystemLibrary.h"

class FDeviceComandTask {

private:

	UASerialLibControllerWin* Device;

	void SetKeepRawData(double* data, KeepRawData& KRD) {

		int index = 0;

		for (int i = 0; i < 3; i++) {

			KRD.acc[i] = data[index];
			index++;
		}

		for (int i = 0; i < 3; i++) {

			KRD.gyr[i] = data[index];
			index++;
		}

		for (int i = 0; i < 3; i++) {

			KRD.mag[i] = data[index];
			index++;
		}
	}

public:

	FDeviceComandTask(UASerialLibControllerWin* d_) {

		Device = d_;
	}

	void GetSeneserRotation(int senserNum, KeepRawData& KRD) {

		if (senserNum < 1 || senserNum > 4) { 

			UE_LOG(LogTemp, Error, TEXT("index is Non"))
			return;
		}

		uint8_t index[3] = {
			0x20,
			0x21,
			0x22
		};

		Device->WriteData(index[senserNum - 1]);
		ASerialDataStruct::ASerialData ReceiveData;

		int Result = Device->ReadData(&ReceiveData);

#ifdef UE_DEBUG_LOG

		// ログ
		uint16_t Error = Device->GetLastErrorCode();
		UE_LOG(LogTemp, Log, TEXT("Error  : %X"), Error);
		UE_LOG(LogTemp, Log, TEXT("Contact  : %d"), Result);
		UE_LOG(LogTemp, Log, TEXT("Result  ; %x"), sizeof(ReceiveData.data));

#endif // UE_DEBUG_LOG

		if (Result == 0) {

			RawDataCalculator rawData;

			rawData.SetReciveData(ReceiveData.data);

			double d[9] = { 0,0,0,0,0,0,0,0,0 };
			rawData.GetReciveData(d);

			SetKeepRawData(d, KRD);
		}

		return;
	}
};

// Sets default values
AASerialReceiverActor::AASerialReceiverActor():
	index(0)
{
 	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 3; i++) {

		DeviceRotate[i] = FRotator(0, 10000, 0);//初期値はでたらめ
		DeviceQuat[i] = FQuat(0, 0, 0, 0);

		SenserData[i] = { 0,0,0 };

		const double Nomarize = 1.0 / 20.0;
		sd[i] = SerialData(Nomarize, Nomarize, Nomarize);
	}
	
	handle = 0;
}

// Called when the game starts or when spawned
void AASerialReceiverActor::BeginPlay()
{
	Super::BeginPlay();
	
	SerialController = NewObject<UASerialLibControllerWin>(this);
	SerialInterface = new WindowsSerial();
	SerialController->Initialize(0x04, 0x03);
	SerialController->SetInterfacePt(SerialInterface);

	if (SerialController->AutoConnectDevice(handle) == ConnectResult::Succ)
	{
		IsDeviceConnected = true;
		UE_LOG(LogTemp, Log, TEXT("Device connected successfully."));

		// メモリのクリア
		PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

		SerialController->WriteData(0x00);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to auto-connect to device."));
	}
	
	// メモリのクリア
	PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

	DCT = new FDeviceComandTask(SerialController);

	for (int i = 0; i < 3; i++) {

		sd[i].setTauAcc(0.1); 
		sd[i].setTauMag(0);
	}
}

void AASerialReceiverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeviceCnt += DeltaTime;
	int i = index % 3;

	// 接続できてなかったら処理なし
	if (!IsDeviceConnected) return;

	// キャリブレーション中
	if (IsBiasCalculated != true) {

		DCT->GetSeneserRotation(i + 1, SenserData[i]);

		GyrBias[i][0] += SenserData[i].gyr[0];
		GyrBias[i][1] += SenserData[i].gyr[1];
		GyrBias[i][2] += SenserData[i].gyr[2];
		BiasCount++;

		if (BiasCount >= BiasSampleCount)
		{
			GyrBias[i][0] /= BiasSampleCount / 3;
			GyrBias[i][1] /= BiasSampleCount / 3;
			GyrBias[i][2] /= BiasSampleCount / 3;
			IsBiasCalculated = true;

			UE_LOG(LogTemp, Log, TEXT("Calculated end"));
		}
		return;
	}

	// 角度を取得する
	if (DeviceCnt >= MaxDeviceCnt) {
	
		DeviceCnt = 0;

		// データの処理
		DCT->GetSeneserRotation(i + 1, SenserData[i]);

		// キャリブレーションに合わせて調整
		constexpr double DEG_TO_RAD = 3.141592653589793 / 180.0;
		double correctedGyr[3] = {
			(SenserData[i].gyr[0] - GyrBias[i][0]) * DEG_TO_RAD,
			(SenserData[i].gyr[1] - GyrBias[i][1]) * DEG_TO_RAD,
			(SenserData[i].gyr[2] - GyrBias[i][2]) * DEG_TO_RAD
		};

		sd[i].update(correctedGyr, SenserData[i].acc, SenserData[i].mag);

		if (index == 0) {

			double q[4];
			sd[i].getQuat9D(q);
			DeviceQuat[i] = FQuat(q[0], q[1], q[2], q[3]);
		}
		else {

			double q[4];
			sd[i].getQuat6D(q);
			DeviceQuat[i] = FQuat(q[0], q[1], q[2], q[3]);
		}

		if (!bInitQuatSet && index == 2) // 3デバイス取得完了時
		{
			for (int j = 0; j < 3; j++)
				InitQuat[j] = DeviceQuat[j];

			bInitQuatSet = true;
		}
	}

	index++;
	if (index >= 3) index = 0;
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

	if (DCT) {

		delete DCT;
		DCT = nullptr;
	}

	Super::EndPlay(EndPlayReason);

}

FRotator AASerialReceiverActor::GetRotation(int s_) {

	if (s_ < 1 || s_ > 4) {

		return FRotator::ZeroRotator;
	}

	return DeviceRotate[s_ - 1];
}

void AASerialReceiverActor::GetDeviceRotate(FRotator* r, int size) {

	int RotateSize= sizeof(DeviceRotate) / sizeof(DeviceRotate[0]);

	if (size != RotateSize) { return; }

	for (int n = 0; n < RotateSize; n++)
		r[n] = DeviceRotate[n];
}

// インターフェース

void AASerialReceiverActor::GetDeviceData(SenserType type, FQuat& quat) {

	if (SerialController == nullptr) { return; }

	quat = DeviceQuat[int(type)];
}

void AASerialReceiverActor::GetDeviceData(SenserType type, FRotator& rot) {

	if (SerialController == nullptr) { return; }
	if (!bInitQuatSet) { return; }

	FRotator r = (DeviceQuat[int(type)] * InitQuat[int(type)].Inverse()).Rotator();
	
	rot = r;
}
