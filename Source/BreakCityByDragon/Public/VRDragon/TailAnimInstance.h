// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TailAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BREAKCITYBYDRAGON_API UTailAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	FRotator TailBoneRotation_Senser1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	FRotator TailBoneRotation_Senser2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	FRotator TailBoneRotation_Senser3;
};
