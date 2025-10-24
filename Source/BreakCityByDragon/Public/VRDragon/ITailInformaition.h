// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITailInformaition.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UITailInformaition : public UInterface
{
	GENERATED_BODY()
};

/**
 * アクセス用のインターフェース
 */
class BREAKCITYBYDRAGON_API IITailInformaition
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FVector GetTargetActorLocation() = 0;
	virtual FRotator GetTargetActorRotation() = 0;
	virtual FVector GetTargetActorForwardVector() = 0;
};
