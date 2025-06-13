// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InputActionValue.h"
#include "VRDragon_ver2.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class UInputMappingContext;
class UInputAction;
class USphereComponent;

UCLASS()
class BREAKCITYBYDRAGON_API AVRDragon_ver2 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRDragon_ver2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	/** Character用のStaticMesh : Sphere */
	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Player;

	UPROPERTY(VisibleAnywhere, Category = Collider, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	// コントローラーのマッピング
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ControlMove;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ControlFire;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category = Control, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> Arrow;

private:

	// 接触判定の処理、コライダー同士が接触したときに呼び出される
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 接触判定の処理、コライダー同士が離れたときに呼び出される
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	// playerコントロール
	void ControlPlayer(const FInputActionValue& Value);

	// 火球コントロール
	void GoFire(const FInputActionValue& Value);

	// カメラコントロール
	void Look(const FInputActionValue& Value);

private:

	float MoveSpeedPoint = 10.0f;
};
