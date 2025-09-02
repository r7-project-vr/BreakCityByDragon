// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/ScoreGameInstance.h"

void UScoreGameInstance::SaveScoreInformaiton(int s_) {

	if (!scoreInfo) {

		scoreInfo = new ScoreInformation();
	}

	scoreInfo->AddScore(s_);
}

int UScoreGameInstance::GetScore() {

	int score = 0;

	if (scoreInfo) { 
		 
		score = scoreInfo->GetScore();
	}

	return score;
}