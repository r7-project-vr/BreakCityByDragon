// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASerial/ASerialReceiverActor.h"
#include "VRDragon/TailAnimInstance.h"
#include "TailActor_ver2.generated.h"

UCLASS()
class BREAKCITYBYDRAGON_API ATailActor_ver2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATailActor_ver2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 初期角度調整用
	bool ResetRotation(FRotator* r, int index);

	// 角度を取得する
	void SetDeviceRotate(FRotator* r,int size);

private:

	// 非同期でメッシュを読み込む
	void LoadMeshAsync();

	// 非同期でアニメBPを読み込む
	void LoadAnimBPAsync();

	void OnAnimBPLoaded();

	// メッシュ読み込み後に呼ばれるコールバック関数
	void OnMeshLoaded();

	// 角度調整用
	void SetRotation(FRotator& r);

	// 尻尾の回転
	void UpdateTailRotation();

	void CheckTailBonesValid();

public:

	UPROPERTY(EditAnywhere, Category = USkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<USkeletalMesh> SoftSkeletalMeshRef;

	UPROPERTY(EditAnywhere, Category = AnimBPClassRef, meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UAnimInstance> AnimBPClassRef;

	UPROPERTY(EditAnywhere, Category = USkeletalMeshComponent, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY()
	UTailAnimInstance* TailInstance;

private :

	// 変数

	FRotator DeviceRotate[3];
	FRotator FirstRotate[3];
	bool FirstRotateSetFlag;
	bool bCheckSkeltalMeshInstancePending = false;
	bool bCheckAnimInstancePending = false;
	FTimerHandle AnimInstanceTimerHandle;

	// 定数

	const float PitchProduct = (1.f / 5.f);
	const float RollProduct = 2.f;
};