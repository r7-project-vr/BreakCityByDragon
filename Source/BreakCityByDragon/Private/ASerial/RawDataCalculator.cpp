// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/RawDataCalculator.h"

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

	// データを分割
	DivisionRawData divisionRawData;
	divisionRawData = DivisionData(data);

	// データを変換して格納
	for (int i = 0; i < 3; i++) {

		acc[i] = I_uintToDouble(divisionRawData.acc[i]);
		gyr[i] = I_uintToDouble(divisionRawData.gyr[i]);
		mag[i] = I_uintToDouble(divisionRawData.mag[i]);
	}
}

bool RawDataCalculator::GetReciveData(double* data) {

	int index = 0;

	for (int i = 0; i < 3; i++) {

		data[index] = acc[i] / 100;
		index++;
	}

	for (int i = 0; i < 3; i++) {

		data[index] = gyr[i] / 100;
		index++;
	}

	for (int i = 0; i < 3; i++) {

		data[index] = mag[i] / 100;
		index++;
	}
	
	return true;
}

// -------------------------------------------
// private
// -------------------------------------------

DivisionRawData RawDataCalculator::DivisionData(const uint8_t* data) {

	DivisionRawData divisionRawData;

	// 分割用の構造体に合わせる
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
