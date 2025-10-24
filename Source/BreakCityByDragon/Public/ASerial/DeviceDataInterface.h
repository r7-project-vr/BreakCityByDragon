// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeviceDataInterface.generated.h"

enum class SenserType {

	Senser1,	// 9Ž²
	Senser2,	// 6Ž²
	Senser3		// 6Ž²
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeviceDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BREAKCITYBYDRAGON_API IDeviceDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void GetDeviceData(SenserType type, FQuat& quat) = 0;

	virtual void GetDeviceData(SenserType type, FRotator& rot) = 0;
};
