// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "DamegeTextUI.generated.h"

/**
 *
 */
UCLASS()
class BREAKCITYBYDRAGON_API UDamegeTextUI : public UWidget
{
	GENERATED_BODY()

protected:

	bool Initialize();

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlockDamage;
};
