// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerialLibControllerWin.h"
#include "Containers/Queue.h"
#include "ASerial/ASerialFunc.h"
#include "ASerial/DeviceRotation.h"
#include "ASerial/RawDataCalculator.h"
#include "ASerial/SerialData.h"
#include "ASerial/DeviceDataInterface.h"
#include "ASerialReceiverActor.generated.h"

class UASerialLibControllerWin;
class FRunnableThread;
class WindowsSerial;

#define UE_DEBUG_LOG false;

// センサーのデータを保存する構造体
struct KeepRawData {

	double gyr[3];
	double acc[3];
	double mag[3];
};

UCLASS()
class BREAKCITYBYDRAGON_API AASerialReceiverActor : public AActor,public IDeviceDataInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASerialReceiverActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void GetDeviceData(SenserType type, FQuat& quat)override;

	virtual void GetDeviceData(SenserType type, FRotator& rot)override;

	FRotator GetRotation(int SencerNum);

	// 通信関係の関数を管理するクラス
	ASerialFunc* SerialFunc;

	// 角度を取得する
	void GetDeviceRotate(FRotator* r, int size);

public :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	FVector DeviceEulerAngles;

private:

	UPROPERTY()
	TObjectPtr<UASerialLibControllerWin> SerialController;

	WindowsSerial* SerialInterface;

	FQuat		DeviceQuat[3];
	FQuat		InitQuat[3]{
		FQuat(10000,0,0,0),
		FQuat(10000,0,0,0),
		FQuat(10000,0,0,0),
	};
	FRotator	DeviceRotate[3];
	bool		IsDeviceConnected;
	bool        bInitQuatSet = false;

	DeviceRotation* DR;
	HANDLE handle;

	bool IsCalibration = true;
	float DeviceCnt = 0;

	int senserNumver;

	SerialData sd[3];
	KeepRawData SenserData[3];

	double GyrBias[3][3] = { 
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};
	bool IsBiasCalculated = false;
	int BiasCount = 0;
	
	// メソッド

	bool CalibrationDevice(int index,ASerialDataStruct::ASerialData& ReceiveData);

	void GetSenserRotaition(int index, ASerialDataStruct::ASerialData& ReceiveData);

	void SetKeepRawData(double* data, KeepRawData& KRD);

	// const変数

	const float MaxDeviceCnt = 1.0f / 60.f;
	const float MaxCalibrationTime = 2.0f;
	const int BiasSampleCount = 200; // 約2秒分（120Hzなら
};
