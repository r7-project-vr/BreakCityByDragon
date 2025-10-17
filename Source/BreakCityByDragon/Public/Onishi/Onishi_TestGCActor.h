// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "Onishi_TestGCActor.generated.h"

/**
 *
 */
UCLASS()
class BREAKCITYBYDRAGON_API AOnishi_TestGCActor : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	AOnishi_TestGCActor();

protected:
	virtual void BeginPlay() override;

private:
	//UPROPERTY()
	//UGeometryCollectionComponent* GCComponent;
};
