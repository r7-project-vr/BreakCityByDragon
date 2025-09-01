// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BREAKCITYBYDRAGON_API ScoreInformation
{
public:
	ScoreInformation();
	~ScoreInformation();

private :

	int Score;

public :

	// 加算メソッド
	void AddScore(int score);

	// 参照メソッド
	int GetScore();
};
