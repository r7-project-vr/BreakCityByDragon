// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASerial/ASerialFunc/I_ASerialFunc.h"

/**
 * 
 */
class BREAKCITYBYDRAGON_API RotationData : public I_ASerialFunc
{
private :

	int senserNum;

public:
	RotationData(UASerialLibControllerWin* d_, int num);
	void SerialFunc() override;
	int SerialRead(ASerialDataStruct::ASerialData& ReceiveData) override;
};
