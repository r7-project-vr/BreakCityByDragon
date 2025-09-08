// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Score/ScoreInformation.h"
#include "ScoreGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BREAKCITYBYDRAGON_API UScoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public :

	ScoreInformation* scoreInfo;

	// スコアクラスのインスタンス化
	void CreateScoreInformaito();

	// 保存用メソッド
	void SaveScoreInformaiton(int score);

	// 参照メソッド
	UFUNCTION()
	int GetScoreInformaiton();

	UFUNCTION()
	void DeleteScoreInformaiton();
};
