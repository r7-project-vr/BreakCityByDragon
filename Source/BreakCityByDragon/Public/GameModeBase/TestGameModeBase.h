// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TestGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class BREAKCITYBYDRAGON_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATestGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	void ToResultGame();

private:

	float GameTimeCnt;

	// ----------------------
	// íËêî
	// ----------------------

	const float MaxGameTime = 30.f;

};
