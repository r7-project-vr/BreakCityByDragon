// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/DeviceRotation.h"

DeviceRotation::DeviceRotation()
{
	RotateSize = sizeof(DeviceRotate) / sizeof(DeviceRotate[0]);

	for (int n = 0; n < RotateSize; n++) {

		FRotator r = FRotator::ZeroRotator;
		DeviceRotate[n] = r;
	}
}

DeviceRotation::~DeviceRotation()
{

}

void DeviceRotation::GetDeviceRotate(FRotator *r) {

	int size = sizeof(r) / sizeof((r)[0]);

	if (size != RotateSize) { return; }

	for (int n = 0; n < RotateSize; n++)
	*(r) = DeviceRotate[n];
}
