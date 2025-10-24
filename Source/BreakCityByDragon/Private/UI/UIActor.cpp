// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIActor.h"
#include "Score/ScoreGameInstance.h"

// Sets default values
AUIActor::AUIActor():
	Score(0),
	TimeCnt(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネントの作成
	{
		USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
		RootComponent = root;
	}
	
	// メッシュの生成
	/*{
		UStaticMesh* Box = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		mesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		mesh->SetupAttachment(RootComponent);
		mesh->SetStaticMesh(Box);
	}*/
}

// Called when the game starts or when spawned
void AUIActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUIActor::Destroyed() {

	Super::Destroyed();

	// Scoreの保存
	if (UGameInstance* GI = GetGameInstance()){

		if (GI) {

			UScoreGameInstance* MyGI = Cast<UScoreGameInstance>(GI);

			if (MyGI){
				MyGI->SaveScoreInformaiton(Score);
			}
		}
	}
}

// Called every frame
void AUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 時間を測る
	TimeCnt += DeltaTime;

	// 上に向かっていく処理
	{
		FVector nowLocation = GetActorLocation();
		SetActorLocation(nowLocation + addVec);
	}

	// 回転の処理
	{
		FRotator CurrentRotation = GetActorRotation();

		CurrentRotation.Yaw += RotaitionValue * DeltaTime;

		SetActorRotation(CurrentRotation);
	}

	// 一定の時間になったらアクターを破壊
	{
		if (TimeCnt >= MaxTimeCnt) {

			this->Destroy();
		}
	}
}

void AUIActor::SetScore(int score_) {
	Score = score_;
}