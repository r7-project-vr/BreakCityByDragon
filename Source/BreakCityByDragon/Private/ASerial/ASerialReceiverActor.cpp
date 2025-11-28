// Fill out your copyright notice in the Description page of Project Settings.

#include "ASerial/ASerialReceiverActor.h"
#include "ASerialLibControllerWin.h"
#include "WindowsSerial/WindowsSerial.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ASerialCore/ASerialPacket.h"
#include "ASerial/RawDataCalculator.h"
// 仮
#include "ASerial/ASerialFunc/RotationData.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AASerialReceiverActor::AASerialReceiverActor() :
	senserNumver(0),
	SerialFunc(nullptr)
{
 	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 3; i++) {

		DeviceRotate[i] = FRotator(0, 10000, 0);//初期値はでたらめ
		DeviceQuat[i] = FQuat(0, 0, 0, 0);

		SenserData[i] = { 0,0,0 };
		double nomarize = 1.0 / 20.0;
		sd[i] = SerialData(nomarize, nomarize, nomarize);
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

	SerialFunc = new ASerialFunc(SerialController);

	for (int i = 0; i < 3; i++) {

		sd[i].setTauAcc(5.0); 
		sd[i].setTauMag(10.0);
	}
}

void AASerialReceiverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeviceCnt += DeltaTime;

	// 接続できてなかったら処理なし
	if (!IsDeviceConnected) return;

	// 通信処理
	{
		TSharedPtr<I_ASerialFunc> f = SerialFunc->GetFunc();

		if (f.IsValid())
		{
			f->SerialFunc();

			ASerialDataStruct::ASerialData ReceiveData;
			int result = f->SerialRead(ReceiveData);

			// 角度の処理
			if (result == 0)
			{
				// キャリブレーション中の処理
				if (!IsBiasCalculated)
					IsBiasCalculated = CalibrationDevice(senserNumver, ReceiveData);

				// キャリブレーション後の処理
				if (IsBiasCalculated)
				{
					GetSenserRotaition(senserNumver, ReceiveData);
				}
			}
		}
	}

#if true

	// 仮の処理
	if (DeviceCnt >= MaxDeviceCnt) 
	{
		TSharedPtr<I_ASerialFunc> funk = MakeShared<RotationData>(SerialController, senserNumver + 1);
		SerialFunc->PrioritizeAddFunc(funk);// 回転取得コマンドは優先に回す

		DeviceCnt = 0;
	}
	
#endif

	senserNumver++;
	if (senserNumver >= 3) senserNumver = 0;
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

void AASerialReceiverActor::GetDeviceRotate(FRotator* r, int size) {

	int RotateSize= sizeof(DeviceRotate) / sizeof(DeviceRotate[0]);

	if (size != RotateSize) { return; }

	for (int n = 0; n < RotateSize; n++)
		r[n] = DeviceRotate[n];
}

bool AASerialReceiverActor::CalibrationDevice(int i, ASerialDataStruct::ASerialData& ReceiveData)
{
	RawDataCalculator rawData;

	rawData.SetReciveData(ReceiveData.data);

	double d[9] = { 0,0,0,0,0,0,0,0,0 };
	rawData.GetReciveData(d);

	SetKeepRawData(d, SenserData[i]);

	GyrBias[i][0] += SenserData[i].gyr[0];
	GyrBias[i][1] += SenserData[i].gyr[1];
	GyrBias[i][2] += SenserData[i].gyr[2];
	BiasCount++;

	if (BiasCount >= BiasSampleCount)
	{
		GyrBias[i][0] /= BiasSampleCount;
		GyrBias[i][1] /= BiasSampleCount;
		GyrBias[i][2] /= BiasSampleCount;

		UE_LOG(LogTemp, Log, TEXT("Calculated end"));

		return true;
	}

	return false;
}

void AASerialReceiverActor::GetSenserRotaition(int index, ASerialDataStruct::ASerialData& ReceiveData)
{
	// キャリブレーションに合わせて調整
	constexpr double DEG_TO_RAD = 3.141592653589793 / 180.0;
	double correctedGyr[3] = {
		(SenserData[index].gyr[0] - GyrBias[index][0]) * DEG_TO_RAD,
		(SenserData[index].gyr[1] - GyrBias[index][1]) * DEG_TO_RAD,
		(SenserData[index].gyr[2] - GyrBias[index][2]) * DEG_TO_RAD
	};

	sd[index].update(correctedGyr, SenserData[index].acc, SenserData[index].mag);

	if (index == 0) {

		double q[4];
		sd[index].getQuat9D(q);
		DeviceQuat[index] = FQuat(q[1], q[2], q[3], q[0]);
	}
	else {

		double q[4];
		sd[index].getQuat6D(q);
		DeviceQuat[index] = FQuat(q[1], q[2], q[3], q[0]);
	}

	if (!bInitQuatSet && index == 2) // 3デバイス取得完了時
	{
		for (int j = 0; j < 3; j++)
			InitQuat[j] = DeviceQuat[j];

		bInitQuatSet = true;
	}
}

void AASerialReceiverActor::SetKeepRawData(double* data, KeepRawData& KRD) {

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
