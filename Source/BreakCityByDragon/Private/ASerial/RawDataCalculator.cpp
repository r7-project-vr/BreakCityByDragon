// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/RawDataCalculator.h"

// -------------------------------------------
// public
// -------------------------------------------

RawDataCalculator::RawDataCalculator()
{
	// メンバー変数の初期化

	for (int i = 0; i < 3; i++) {

		gyr[i] = 0;
		acc[i] = 0;
		mag[i] = 0;
	}
}

RawDataCalculator::~RawDataCalculator()
{
}

void RawDataCalculator::SetReciveData(const uint8* data, ESenserType type) {

	// 18バイト分だけコピー
	TArray<uint8> RawData;
	RawData.Append(data, 18);

	// 構造体に格納
	receiveData.rawData = RawData;

	// データを分割
	DivisionData();

	// データを変換して格納
	for (int i = 0; i < 3; i++) {

		acc[i] = I_uintTodouble(receiveData.divRawData.acc[i]);
		gyr[i] = I_uintTodouble(receiveData.divRawData.gyr[i]);
		mag[i] = I_uintTodouble(receiveData.divRawData.mag[i]);
	}
}

bool RawDataCalculator::GetReciveData(double* data) {

	double clereData[9];

	int index = 0;

	for (int i = 0; i < 3; i++) {

		clereData[index] = receiveData.divRawData.gyr[i];
		index++;
	}

	for (int i = 0; i < 3; i++) {

		clereData[index] = receiveData.divRawData.acc[i];
		index++;
	}

	for (int i = 0; i < 3; i++) {

		clereData[index] = receiveData.divRawData.mag[i];
		index++;
	}

	data = clereData;
	
	return true;
}

// -------------------------------------------
// private
// -------------------------------------------

void RawDataCalculator::DivisionData() {

	DivisionRawData divisionRawData;

	// 分割用の構造体に合わせる
	FMemory::Memcpy(&divisionRawData, receiveData.rawData.GetData(), sizeof(DivisionRawData));

	receiveData.divRawData = divisionRawData;
}

double RawDataCalculator::I_uintTodouble(uint16 data) {

	double value = static_cast<double>(data);

	return value;
}
