// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Score/ScoreInformation.h"
#include "UIActor.generated.h"

UCLASS()
class BREAKCITYBYDRAGON_API AUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIActor();

	UPROPERTY(VisibleAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private :

	// ----------------------------------
	// ブルプリで編集できる変数
	// ----------------------------------

	// スコアの値
	UPROPERTY(EditAnywhere, Category = "ScoreValue")
	int Score;

	
	// ----------------------------------
	// その他変数
	// ----------------------------------

	float				TimeCnt;	// 時間のカウンター

	// ----------------------------------
	// 定数
	// ----------------------------------

	const FVector	addVec			= FVector(0, 0, 1.5f);	// 上昇率
	const float		MaxTimeCnt		= 1.0f;					// 存在できる時間(秒)
	const float		RotaitionValue	= 90.f;					// 回転の量
};
