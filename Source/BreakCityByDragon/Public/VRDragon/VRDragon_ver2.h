// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MotionControllerComponent.h"
#include "InputActionValue.h"
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

protected:

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

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ESCAction;

	UPROPERTY(VisibleAnywhere, Category = Control, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> BlueprintFireBall;

	// 腕のメッシュ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> LMesh; // 左腕

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RMesh; // 右腕

	UPROPERTY(EditAnywhere, Category = LSoftSkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> LStaticMeshRef;

	UPROPERTY(EditAnywhere, Category = RSoftSkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> RStaticMeshRef;

	UPROPERTY(EditAnywhere, Category = Tail_BP_Class, meta = (AllowPrivateAccess = "true"))
	UClass* TailBP;

	UPROPERTY(EditAnywhere, Category = TailActor, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ATailActor_ver2> tails;// 尻尾

private:

	// 接触判定の処理、コライダー同士が接触したときに呼び出される
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLHandBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRHandBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	// playerコントロール
	void ControlPlayer(const FInputActionValue& Value);

	// 火球コントロール
	void GoFire(const FInputActionValue& Value);

	// カメラコントロール
	void Look(const FInputActionValue& Value);

	// カメラコントロール
	void ESCtoStart(const FInputActionValue& Value);

	// VRカメラ
	bool GetHMDPose(FVector& OutPosition, FRotator& OutRotation);

	// 座標制御
	void CheckVec(FVector& PlayerNextVec);

	void MovePlayer(float DeltaTime,FRotator DeviceRotate);

	void PlayControllerHaptic(APlayerController* PlayerController, UHapticFeedbackEffect_Base* H_, EControllerHand Hand);

private:

	float MoveSpeedPoint = 150.0f;

	bool CanFire;

	bool IsSetFirstRotation;

	float FireChargeCnt;

	int tailLength = 1;

	FVector preTailVec;

	UPROPERTY()
	TObjectPtr<AASerialReceiverActor> ASerialReceiverActor;

	UPROPERTY()
	UHapticFeedbackEffect_Base* HapticEffect;
	

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

	//Debug用にpublic化 大西
	public:
		UPROPERTY(BluePrintReadWrite)
		float addpow = 0;
};
