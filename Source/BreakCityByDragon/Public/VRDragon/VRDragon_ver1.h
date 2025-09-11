// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "MotionControllerComponent.h"
#include "InputActionValue.h"
#include "VRDragon_ver1.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AFireBall_ver2;

UCLASS()
class BREAKCITYBYDRAGON_API AVRDragon_ver1 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRDragon_ver1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// 火球コントロール
	void GoFire(const FInputActionValue& Value);

	TWeakObjectPtr<AFireBall_ver2> FB;

protected:

	// カメラ
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	// コントローラーのマッピング
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ControlFire;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> BlueprintFireBall;
};
