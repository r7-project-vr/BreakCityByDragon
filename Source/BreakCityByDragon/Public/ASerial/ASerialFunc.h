// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASerial/ASerialFunc/I_ASerialFunc.h"

#define MAX_FuncIndex 10

// 送る関数の親クラス
enum class ASerialFuncName 
{
	None,
	RotationData,
	SetLightColor,
	ClearLightColor,
	SetLightAnim,
	ClearLightAnim,
	SetLightLumina,
	SetFPSAnim
};

// 関数群を管理するクラス
class BREAKCITYBYDRAGON_API ASerialFunc
{
public:

	ASerialFunc(UASerialLibControllerWin* d_);

	~ASerialFunc();

	// [0]の関数を呼び出して削除し、配列をずらす
	TSharedPtr<I_ASerialFunc> GetFunc();

	// [0]の配列にねじ込む
	void PrioritizeAddFunc(TSharedPtr<I_ASerialFunc> name);

	// 一番後ろの配列に関数を追記する
	void AddFunc(TSharedPtr<I_ASerialFunc> name);

private :

	// メンバー

	UASerialLibControllerWin* Device; // デバイスのポインタ

	TSharedPtr<I_ASerialFunc> funcPool[MAX_FuncIndex];// 関数管理用

	// メソッド
	
	//[0]の関数を削除する、引数があれば指定された配列に代入する
	void DeleteFunc(int index = 0) { funcPool[index] = nullptr; }

	// 配列を並び変える
	// 0,2,1,0,3,2
	//    ->
	// 2,1,3,2,0,0
	void SwapFunc()
	{
		for (int j = 1; j < MAX_FuncIndex; j++) {
			for (int i = j; i < MAX_FuncIndex; i++)
			{
				if (funcPool[i - 1] != nullptr) { continue; }

				TSharedPtr<I_ASerialFunc> pre = funcPool[i - 1];
				funcPool[i - 1] = funcPool[i];
				funcPool[i] = pre;
			}
		}
	}

	// 配列を任意の数だけずらす
	void ShiftFunk(int ShiftIndex)
	{
		auto preFunc = funcPool;

		for (int i = 1; i < MAX_FuncIndex; i++)
		{
			funcPool[i - 1] = preFunc[i];
		}

		funcPool[0] = nullptr;
	}
};
