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
	void CreateScoreInformation();

	// 保存用メソッド
	void SaveScoreInformaiton(int score);

	// 参照メソッド
	UFUNCTION(BlueprintCallable)
	int GetScoreInformaiton();

	UFUNCTION()
	void DeleteScoreInformaiton();
};
