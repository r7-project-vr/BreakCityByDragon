// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ATailActor::ATailActor() :
	MoveVec(FVector::ZeroVector),
	DeviceVec(FVector::ZeroVector)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
	UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	TailMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	TailMesh->SetStaticMesh(Sphere);
	TailMesh->SetupAttachment(RootComponent);
	TailMesh->SetSimulatePhysics(false);

	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	TailMesh->SetMaterial(0, Material);

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

	// 親アクターがnullなら処理なし
	{
		if (TargetActor == nullptr) {
			return;
		}
	}

	{
		FVector rig = TargetActor->GetTargetActorForwardVector();
		//TailMove(rig,DeltaTime);
		TailRotaiton(DeltaTime);
	}
}

void ATailActor::SetParentTail(AActor* tail) {

	TargetActor = Cast<IITailInformaition>(tail);
}

// しっぽの追従プログラム
void ATailActor::TailMove(FVector rig, float deltaTime) {

	// TargetActorに追従する
	{
		FVector ToTarget = TargetActor->GetTargetActorLocation() - GetActorLocation();
		float Distance = ToTarget.Size();

		if (Distance > maxTailLenge) {

			FVector NewLocation = FMath::VInterpTo(
				GetActorLocation(),
				TargetActor->GetTargetActorLocation() - ToTarget.GetSafeNormal() * maxTailLenge,
				deltaTime,
				FollowSpeed
			);

			SetActorLocation(NewLocation);
		}
	}
}

// しっぽの角度調整プログラム
void ATailActor::TailRotaiton(float deltaTime) {

	// TargetActorの角度になる
	{
		SetActorRotation(TargetActor->GetTargetActorRotation());
	}

	// TargetActorの真後ろに追従する
	{
		FVector TargetLocation = TargetActor->GetTargetActorLocation();

		float DesiredOffset = maxTailLenge;// 調整用
		FVector TargetFollowLocation = TargetLocation - TargetActor->GetTargetActorForwardVector() * DesiredOffset;

		FVector NewLocation = FMath::VInterpTo(
			GetActorLocation(),
			TargetFollowLocation,
			deltaTime,
			FollowSpeed
		);

		SetActorLocation(NewLocation);
	}
}

FVector ATailActor::GetTargetActorLocation() {

	return this->GetActorLocation();
}

// しっぽの角度を返す
FRotator ATailActor::GetTargetActorRotation() {

	return GetActorRotation();
}

FVector ATailActor::GetTargetActorForwardVector() {

	return GetActorForwardVector();
}
