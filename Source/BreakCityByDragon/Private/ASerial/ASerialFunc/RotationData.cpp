// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/ASerialFunc/RotationData.h"

RotationData::RotationData(UASerialLibControllerWin* d_, int num):
	I_ASerialFunc(d_) ,
	senserNum(num)
{
}

void RotationData::SerialFunc() 
{
	if (senserNum < 1 || senserNum > 4) {

		UE_LOG(LogTemp, Error, TEXT("index is Non"))
			return;
	}

	uint8_t index[3] = {
		0x20,
		0x21,
		0x22
	};

	Device->WriteData(index[senserNum - 1]);
}

int RotationData::SerialRead(ASerialDataStruct::ASerialData& ReceiveData)
{
	int Result = Device->ReadData(&ReceiveData);
	return Result;
}
