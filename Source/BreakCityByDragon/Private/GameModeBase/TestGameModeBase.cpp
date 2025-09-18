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

	// WidgetBlueprintのClassを取得する
	FString Path = TEXT("/Game/Onishi/UI/WBP_Fade.WBP_Fade_C");
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerControllerを取得する
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ATestGameModeBase::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	GameTimeCnt += DeltaTime;

	if (GameTimeCnt >= MaxGameTime) {

		//UE_LOG(LogTemp, Display, TEXT("GameTimeCntが規定値になりました"));

		if (WidgetClass && PlayerController && WidgetSpawn == false) {
			WidgetSpawn = true;

			// Widgetを作成する
			UFadeWidget* UserWidget = CreateWidget<UFadeWidget>(GetWorld(), WidgetClass);

			// Viewportに追加する
			UserWidget->AddToViewport(0);

			UKismetSystemLibrary::PrintString(GEngine->GetWorld(), "Widget Spawned");

			if (!UserWidget->Fade) {
				UKismetSystemLibrary::PrintString(GEngine->GetWorld(), "Fade is null");
			}
			// アニメーションを再生
			Player = UserWidget->PlayAnimation(UserWidget->Fade);
			if (Player)
			{
				// Finish Event 相当
				Player->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& SeqPlayer)
					{
						UE_LOG(LogTemp, Log, TEXT("FadeIn finished (C++)!"));
						ToResultGame();
					});
			}
		}
		else if(!PlayerController){
			UKismetSystemLibrary::PrintString(GEngine->GetWorld(), "Player Controller is null");
		}
		else if (!WidgetClass) {
			UKismetSystemLibrary::PrintString(GEngine->GetWorld(), "Widget is null");
		}
		
	}
}

void ATestGameModeBase::ToResultGame() {

	UGameplayStatics::OpenLevel(GetWorld(), FName("Onishi_Result"));
	
}