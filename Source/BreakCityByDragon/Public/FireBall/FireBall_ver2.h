// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBall_ver2.generated.h"

UCLASS()
class BREAKCITYBYDRAGON_API AFireBall_ver2 : public AActor
{
	GENERATED_BODY()

public:
	AFireBall_ver2();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "FireBall")
	TObjectPtr<UStaticMeshComponent> FireBall;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
