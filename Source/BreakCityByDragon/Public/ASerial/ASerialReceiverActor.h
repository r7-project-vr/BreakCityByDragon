// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerialLibControllerWin.h"
#include "Containers/Queue.h"
#include "ASerial/DeviceRotation.h"
#include "ASerialReceiverActor.generated.h"

class UASerialLibControllerWin;
class FRunnableThread;
class WindowsSerial;
class FDeviceComandTask;

UCLASS()
class BREAKCITYBYDRAGON_API AASerialReceiverActor : public AActor
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FRotator GetRotation(int SencerNum);

	// Šp“x‚ðŽæ“¾‚·‚é
	void GetDeviceRotate(FRotator* r);

public :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	FVector DeviceEulerAngles;

private:

	UPROPERTY()
	TObjectPtr<UASerialLibControllerWin> SerialController;

	WindowsSerial* SerialInterface;
	class FDeviceComandTask* DCT;
	FRotator DeviceRotate[3];
	bool IsDeviceConnected;

	DeviceRotation* DR;

	bool IsCalibration = true;
	float DeviceCnt = 0;

	const float MaxDeviceCnt = 1.0f / 10.f;
	const float MaxCalibrationTime = 2.0f;
};
