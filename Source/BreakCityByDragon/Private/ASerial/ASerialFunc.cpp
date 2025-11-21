// Fill out your copyright notice in the Description page of Project Settings.


#include "ASerial/ASerialFunc.h"
#include "ASerial/RawDataCalculator.h"

ASerialFunc::ASerialFunc(UASerialLibControllerWin* d_):
	Device(d_)
{
	for (int i = 0; i < 10; i++)
		funcPool[i] = nullptr;
}

ASerialFunc::~ASerialFunc()
{
	// pass
}

void ASerialFunc::GetFunc(I_ASerialFunc& name) 
{
	if (funcPool[0] == nullptr) { return; }

	// 対応した関数を呼び出す
	name = *funcPool[0];

	DeleteFunc();
	SwapFunc();
}

void ASerialFunc::PrioritizeAddFunc(I_ASerialFunc* name) 
{
	// きれいに並び替える
	SwapFunc();

	// [0]配列を開ける
	ShiftFunk(1);

	funcPool[0] = name;
}

void ASerialFunc::AddFunc(I_ASerialFunc* name) 
{

	int i = 0;

	// 配列の最初から参照してあいてる場所に関数を保存する
	while (i < MAX_FuncIndex)
	{
		if (funcPool[i] == nullptr)
		{
			funcPool[i] = name;
			break;
		}
	}

	SwapFunc();
	return;
}

