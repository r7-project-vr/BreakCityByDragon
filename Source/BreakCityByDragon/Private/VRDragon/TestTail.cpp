// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TestTail.h"

// Sets default values
ATestTail::ATestTail()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	TestObj= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	TestObj -> SetupAttachment(RootComponent);

	for (ATailActor* tails:tail) {
		tails = nullptr;
	}
}

// Called when the game starts or when spawned
void ATestTail::BeginPlay()
{
	Super::BeginPlay();

	// 尻尾の生成
	{
		FRotator look = GetControlRotation();

		{
			FVector pos = GetActorLocation();

			tail[0] = GetWorld()->SpawnActor<ATailActor>(ATailActor::StaticClass(), pos, look);
			tail[0]->SetParentTail(this);
			tail[0]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}

		for (int i = 1;i < 6;i++) {

			// しっぽの位置
			FVector pos = GetActorLocation() + FVector(-20.f, 0, 0) * i;
			tail[i] = GetWorld()->SpawnActor<ATailActor>(ATailActor::StaticClass(), pos, look);
			tail[i]->SetParentTail(tail[i - 1]);
			USceneComponent* ParentRootComponent = tail[i - 1]->GetRootComponent();
			tail[i]->AttachToComponent(ParentRootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
}

// Called every frame
void ATestTail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

// Called to bind functionality to input
void ATestTail::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// しっぽの付け根を返す
FVector ATestTail::GetParentActorLocation() {

	return GetActorLocation();
}

// デバイスのオイラー角を送る
FVector* ATestTail::GetParentMoveVector() {

	return &MoveVec;
}

