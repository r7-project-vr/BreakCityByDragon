// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRDragon/TailActor.h"
#include "ITailInformaition.h"
#include "TestTail.generated.h"

UCLASS()
class BREAKCITYBYDRAGON_API ATestTail : public APawn, public IITailInformaition
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestTail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :

	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> TestObj;

public :

	virtual FVector GetParentActorLocation() override;
	virtual FVector GetParentMoveVector() override;
	virtual FVector GetDeviceMoveVector() override;

private :

	ATailActor* tail[6];
	FVector MoveVec;
	FVector DeviceVec;

	int Radians;
	int cnt;

	void moveVec();
};
