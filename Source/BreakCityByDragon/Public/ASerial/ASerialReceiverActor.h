// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerialLibControllerWin.h"
#include "Containers/Queue.h"
#include "ASerialReceiverActor.generated.h"

class UASerialLibControllerWin;
class FRunnableThread;
class WindowsSerial;

struct FSerialData
{
	uint8 command;
	TArray<uint8> data;
};

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

public :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Device Data")
	FVector DeviceEulerAngles;

private:
	void ProcessReceivedData(const FSerialData& Data);

	UPROPERTY()
	TObjectPtr<UASerialLibControllerWin> SerialController;

	WindowsSerial* SerialInterface;
	bool IsDeviceConnected;

	float DeviceCnt = 0;
	const float MaxDeviceCnt = 1.0f / 1.f;
};
