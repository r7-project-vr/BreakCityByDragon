// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ATailActor::ATailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("rootComponent"));
	//RootComponent = root;

	TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
	UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	TailMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	TailMesh->SetStaticMesh(Sphere);

	// Arrowの初期化
	{
		// Arrowを追加する
		Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
		Arrow->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

		// Sphereの頭上に移動するようにLocationを設定する
		Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		// Arrowを表示されるようにする
		Arrow->bHiddenInGame = false;
	}
}

// Called when the game starts or when spawned
void ATailActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATailActor::SetParentTail(ATailActor* tail) {

	TailActorParent = tail;
}

void ATailActor::TailMove(FVector* rig, FRotator* rot){

	// 計算した移動距離を保存する変数
	FVector moveVec = FVector::ZeroVector;// いったんゼロ

	// デバイスに加わっている力に合わせた移動距離を計算する
	{

	}

	// アクターの角度をデバイスの角度にそろえる
	{
		SetActorRotation(*rot);
	}

	// 移動距離と尻尾の存在範囲の処理
	{
		// 尻尾が繋がっている範囲よりも出た場合飛び出た分を減らす
	}

	// アクターの位置の移動
	FVector actorVec = GetActorLocation();
	SetActorLocation(actorVec + moveVec);
}

