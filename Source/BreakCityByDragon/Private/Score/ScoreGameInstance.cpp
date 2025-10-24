// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/ScoreGameInstance.h"

void UScoreGameInstance::CreateScoreInformation() {

	if (!scoreInfo) {

		scoreInfo = new ScoreInformation();
	}
}

void UScoreGameInstance::SaveScoreInformaiton(int s_) {

	if(scoreInfo)
		scoreInfo->AddScore(s_);
}

int UScoreGameInstance::GetScoreInformaiton() {

	int score = 0;

	if (scoreInfo) { 
		 
		score = scoreInfo->GetScore();
	}

	return score;
}

void UScoreGameInstance::DeleteScoreInformaiton(){

	if (scoreInfo)
		scoreInfo = nullptr;
};