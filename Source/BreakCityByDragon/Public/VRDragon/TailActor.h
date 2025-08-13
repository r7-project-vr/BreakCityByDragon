// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h" 
#include "TailActor.generated.h"

class UArrowComponent;

UCLASS()
class BREAKCITYBYDRAGON_API ATailActor : public AActor
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

	void SetParentTail(ATailActor* tail);

	void TailMove(FVector* deviceAcceleration, FRotator* deviceRotate);

public :

	UPROPERTY(VisibleAnywhere, Category = Control, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> Arrow;

private:

	TObjectPtr<UStaticMeshComponent> TailMesh;

	ATailActor* TailActorParent;

	FVector		tailRig;
	FRotator	tailRotate;

	const float tailPosLimit = 0.f;
	const float tailAddPysr = 0.0f;
	const FVector GravityScale = FVector(0, 9.8f, 0) / 1;

	/*
	* このアクターに対して呼び出されたら追従対象を設定する
	*/

	// 接触判定の処理、コライダー同士が接触したときに呼び出される
	//UFUNCTION()
	//void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
