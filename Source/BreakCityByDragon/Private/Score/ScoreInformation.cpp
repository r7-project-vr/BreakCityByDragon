// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/ScoreInformation.h"

ScoreInformation::ScoreInformation():
	Score(0)
{
}

ScoreInformation::~ScoreInformation()
{
}

void ScoreInformation::AddScore(int score) {

	Score += score;
}

int ScoreInformation::GetScore() {

	return this->Score;
}
