// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TestTail.h"

// Sets default values
ATestTail::ATestTail():
	Radians(0),
	cnt(0)
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
			//tail[0]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}

		for (int i = 1;i < 6;i++) {

			// しっぽの位置
			FVector pos = GetActorLocation() + FVector(-20.f, 0, 0) * i;
			tail[i] = GetWorld()->SpawnActor<ATailActor>(ATailActor::StaticClass(), pos, look);
			tail[i]->SetParentTail(tail[i - 1]);
			//USceneComponent* ParentRootComponent = tail[i - 1]->GetRootComponent();
			//tail[i]->AttachToComponent(ParentRootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
}

// Called every frame
void ATestTail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	moveVec();
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
FVector ATestTail::GetParentMoveVector() {

	return MoveVec;
}

FVector ATestTail::GetDeviceMoveVector() {

	FVector vec = DeviceVec;
	return vec;
}

void ATestTail::moveVec() {

	FVector addVec = FVector::Zero();

	int index = Radians % 4;

	switch (index)
	{
	case 0:
		addVec = FVector(1.0f, 0, 0);
		break;
	case 1:
		addVec = FVector(0, 1.0f, 0);
		break;
	case 2:
		addVec = FVector(-1.0f,0, 0);
		break;
	case 3:
		addVec = FVector(0, -1.0f, 0);
		break;
	default:
		break;
	}

	MoveVec = addVec;
	FVector nowVec = GetActorLocation();
	FVector newVec = nowVec + addVec;
	SetActorLocation(newVec);

	cnt++;

	if (cnt >= 10) {

		cnt = 0;
		DeviceVec = FVector(20, 0, 0);
		Radians++;
	}
	else {

		DeviceVec = FVector(0, 0, 0);
	}
}