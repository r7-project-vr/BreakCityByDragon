// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASerialLibControllerWin.h"

/**
 *  送る関数のインターフェース
 */
class BREAKCITYBYDRAGON_API I_ASerialFunc
{
protected:

	UASerialLibControllerWin* Device;

public:

	I_ASerialFunc(UASerialLibControllerWin* d_);

	virtual void SerialFunc() = 0;

	virtual int SerialRead(ASerialDataStruct::ASerialData& ReceiveData) = 0;
};
