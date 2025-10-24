// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerialLibControllerWin.h"
#include "Containers/Queue.h"
#include "ASerial/DeviceRotation.h"
#include "ASerial/RawDataCalculator.h"
#include "ASerial/SerialData.h"
#include "ASerial/DeviceDataInterface.h"
#include "ASerialReceiverActor.generated.h"

class UASerialLibControllerWin;
class FRunnableThread;
class WindowsSerial;
class FDeviceComandTask;

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

	// 角度を取得する
	void GetDeviceRotate(FRotator* r, int size);

public :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	FVector DeviceEulerAngles;

private:

	UPROPERTY()
	TObjectPtr<UASerialLibControllerWin> SerialController;

	WindowsSerial* SerialInterface;
	class FDeviceComandTask* DCT;

	FQuat		DeviceQuat[3];
	FRotator	DeviceRotate[3];
	bool		IsDeviceConnected;

	DeviceRotation* DR;
	HANDLE handle;

	bool IsCalibration = true;
	float DeviceCnt = 0;

	int index;

	SerialData sd[3];
	KeepRawData SenserData[3];

	// const変数

	const float MaxDeviceCnt = 1.0f / 60.f;
	const float MaxCalibrationTime = 2.0f;
};
