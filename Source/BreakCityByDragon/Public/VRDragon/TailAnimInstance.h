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

	UTailAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	TArray<FRotator> TailBoneRotation_Senser;

	// äOÇ©ÇÁèëÇ´ä∑Ç¶â¬î\Ç…Ç∑ÇÈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tail")
	bool bIsTailBonesValid = false;
};
