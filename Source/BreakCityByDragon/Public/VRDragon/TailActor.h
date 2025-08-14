// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h" 
#include "ITailInformaition.h"
#include "TailActor.generated.h"

class UArrowComponent;

UCLASS()
class BREAKCITYBYDRAGON_API ATailActor : public AActor,public IITailInformaition
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATailActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetParentTail(AActor* tail);

	void TailMove(FVector deviceAcceleration);

public :

	UPROPERTY(VisibleAnywhere, Category = Control, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> Arrow;

	virtual FVector GetParentActorLocation() override;
	virtual FVector GetParentMoveVector() override;

private:

	TObjectPtr<UStaticMeshComponent> TailMesh;

	IITailInformaition* TailActorParent;

	FVector MoveVec;
	FVector PreParentLocation;

	// ----------------------------
	// 定数
	// ----------------------------

	const float adjustPow		=	1.0f;	// 調整用の値
	const float maxTailLenge	=	20.0f;	// しっぽの間隔の最大値
	const float minTailLengh	=	15.f;	// しっぽの間隔の最小値

	/*
	* このアクターに対して呼び出されたら追従対象を設定する
	*/

	// 接触判定の処理、コライダー同士が接触したときに呼び出される
	//UFUNCTION()
	//void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
