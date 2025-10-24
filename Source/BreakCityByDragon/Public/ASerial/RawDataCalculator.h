// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESenserType : uint8
{
	Axis_9_Sensor,	// 18
	Axis_6_Sensor	// 12
};

// 分割用の保存用構造体
struct DivisionRawData {

public:

	// ジャイロ
	uint8_t gyr[3];

	// 加速度 
	uint8_t acc[3];

	// 磁力計
	uint8_t mag[3];
};

class BREAKCITYBYDRAGON_API RawDataCalculator
{
public:

	RawDataCalculator();
	~RawDataCalculator();

	void SetReciveData(const uint8_t* data, ESenserType type);

	/// <summary>
	/// 変換後のデータ受け取り
	/// </summary>
	/// <param name="data"></param>
	bool GetReciveData(double* data);

private :

	// メンバー

	DivisionRawData* receiveData;	// 受け取ったデータの格納場所

	double gyr[3];				// ジャイロ
	double acc[3];				// 加速度 
	double mag[3];				// 磁力計

	// メソッド

	/// <summary>
	/// データを分割する
	/// </summary>
	DivisionRawData DivisionData(const uint8_t* data);

	/// <summary>
	/// 生データからdouble型に変換用
	/// </summary>
	/// <param name="data"></param>
	double I_uintTodouble(uint8_t data);
}; 
