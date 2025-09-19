// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/TestGameModeBase.h"
#include "VRDragon/VRDragon_ver2.h"
#include "Score/ScoreGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Onishi/FadeWidget.h"
#include "Kismet/KismetSystemLibrary.h"


ATestGameModeBase::ATestGameModeBase() :
	GameTimeCnt(0) {

	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = AVRDragon_ver2::StaticClass();
}

void ATestGameModeBase::BeginPlay() {
	if (UGameInstance* GI = GetGameInstance()) {

		if (GI) {
			UScoreGameInstance* MyGI = Cast<UScoreGameInstance>(GI);

			if (MyGI) {
				MyGI->CreateScoreInformation();
			}
		}
	}
}

void ATestGameModeBase::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	GameTimeCnt += DeltaTime;

	if (GameTimeCnt >= MaxGameTime) {

		//UE_LOG(LogTemp, Display, TEXT("GameTimeCnt‚ª‹K’è’l‚É‚È‚è‚Ü‚µ‚½"));
	}
}

void ATestGameModeBase::ToResultGame() {

	UGameplayStatics::OpenLevel(GetWorld(), FName("Onishi_Result"));
	
}