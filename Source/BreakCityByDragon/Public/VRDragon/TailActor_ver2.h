// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	// 非同期でメッシュを読み込む
	void LoadMeshAsync();

	// 非同期でアニメBPを読み込む
	void LoadAnimBPAsync();

	// メッシュ読み込み後に呼ばれるコールバック関数
	void OnMeshLoaded();

	// 尻尾の回転
	void UpdateTailRotation();

public:

	TSoftObjectPtr<USkeletalMesh> SoftSkeletalMeshRef;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
