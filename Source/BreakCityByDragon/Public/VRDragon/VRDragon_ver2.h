// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MotionControllerComponent.h"
#include "InputActionValue.h"
#include "VRDragon/TailActor.h"
#include "ITailInformaition.h"
#include "UI/DamageText.h"
#include "ASerial/ASerialReceiverActor.h"
#include "VRDragon/TailActor_ver2.h"
#include "VRDragon_ver2.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class UInputMappingContext;
class UInputAction;
class USphereComponent;
class ATailActor;
class UBoxComponent;

UCLASS()
class BREAKCITYBYDRAGON_API AVRDragon_ver2 : public APawn, public IITailInformaition
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

	virtual FVector GetTargetActorLocation() override;
	virtual FRotator GetTargetActorRotation() override;
	virtual FVector GetTargetActorForwardVector() override;

private:

	/** Character用のStaticMesh : Sphere */
	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Player;

	UPROPERTY(VisibleAnywhere, Category = Collider, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category = Collider, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Body_Base;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> CameraRoot;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	// VRコントローラ
	UPROPERTY(VisibleAnywhere, Category = MotionController, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> LeftMotionController;

	UPROPERTY(VisibleAnywhere, Category = MotionController, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> RightMotionController;

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

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> BlueprintFireBall;

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

	// VRカメラ
	bool GetHMDPose(FVector& OutPosition, FRotator& OutRotation);

	// 座標制御
	void CheckVec(FVector& PlayerNextVec);

	void MovePlayer(float DeltaTime);

	void OnMeshLoaded();

	void LoadMeshAsync();

	// デバイスからの値をゲームに反映させる関数
	void SerialReceiver();

private:

	float MoveSpeedPoint = 200.0f;

	bool CanFire;

	float FireChargeCnt;

	int tailLength = 1;

	FVector newTailVec;
	FVector preTailVec;

	ATailActor_ver2* tails;// 尻尾

	USkeletalMeshComponent* LMesh; // 左腕
	USkeletalMeshComponent* RMesh; // 右腕

	TSoftObjectPtr<USkeletalMesh> LSoftSkeletalMeshRef;
	TSoftObjectPtr<USkeletalMesh> RSoftSkeletalMeshRef;

	USkeletalMesh* LFootMesh; // 左脚
	USkeletalMesh* RFootMesh; // 右脚

	AASerialReceiverActor* ASerialReceiverActor;
	

	// ----------------------------
	// 定数
	// ----------------------------

	FVector CheckMapVec[4] = {

		FVector(),
		FVector(),
		FVector(),
		FVector()
	};

	FVector CheckBossVec[4] = {

		FVector(),
		FVector(),
		FVector(),
		FVector()
	};

public :

		// UI
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UDamageText> DamageTextClass;
};
