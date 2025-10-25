// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/RawDataCalculator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// -------------------------------------------
// public
// -------------------------------------------

RawDataCalculator::RawDataCalculator()
{
	for (int i = 0; i < 3; i++) {

		gyr[i] = 0;
		acc[i] = 0;
		mag[i] = 0;
	}
}

RawDataCalculator::~RawDataCalculator()
{
}

void RawDataCalculator::SetReciveData(const uint8_t* data) {

	for (int i = 0; i < 3; i++) {
		acc[i] = ((int16_t)(data[i * 2] << 8 | data[i * 2 + 1])) / 100.0;      
		gyr[i] = ((int16_t)(data[6 + i * 2] << 8 | data[6 + i * 2 + 1])) / 100.0;
		mag[i] = ((int16_t)(data[12 + i * 2] << 8 | data[12 + i * 2 + 1])) / 100.0; 
	}
}

bool RawDataCalculator::GetReciveData(double* data) {

	int index = 0;

	for (int i = 0; i < 3; i++) {

		data[index] = acc[i];
		index++;
	}

	for (int i = 0; i < 3; i++) {

		data[index] = gyr[i];
		index++;
	}

	for (int i = 0; i < 3; i++) {

		data[index] = mag[i];
		index++;
	}
	
	return true;
}

// -------------------------------------------
// private
// -------------------------------------------

DivisionRawData RawDataCalculator::DivisionData(const uint8_t* data) {

	DivisionRawData divisionRawData;

	// •ªŠ„—p‚Ì\‘¢‘Ì‚É‡‚í‚¹‚é
	int index = 0;

	for (int i = 0; i < 3; i++) {

		divisionRawData.acc[i] = data[index];
		index++;
	}

	for (int i = 0; i < 3; i++) {

		divisionRawData.gyr[i] = data[index];
		index++;
	}

	for (int i = 0; i < 3; i++) {

		divisionRawData.mag[i] = data[index];
		index++;
	}

	return divisionRawData;
}

double RawDataCalculator::I_uintToDouble(uint8_t data) {

	double value = data;
	
	return value;
}
