// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/**
 * 角度を安全に送るためだけのクラス
 * デバイスからこのクラスの角度だけ送る
 */
class BREAKCITYBYDRAGON_API DeviceRotation
{
public:
	DeviceRotation();
	~DeviceRotation();

	// 角度を取得する
	void GetDeviceRotate(FRotator *r);

private:

	FRotator DeviceRotate[3];
	int RotateSize;
};
