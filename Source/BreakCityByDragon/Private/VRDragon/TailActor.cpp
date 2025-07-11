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

	// ArrowÇÃèâä˙âª
	{
		// ArrowÇí«â¡Ç∑ÇÈ
		Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
		Arrow->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

		// SphereÇÃì™è„Ç…à⁄ìÆÇ∑ÇÈÇÊÇ§Ç…LocationÇê›íËÇ∑ÇÈ
		Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		// ArrowÇï\é¶Ç≥ÇÍÇÈÇÊÇ§Ç…Ç∑ÇÈ
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

void ATailActor::TailMove(float x, float y) {

	//FVector AddVec = FVector::Zero();
	//FVector tailLocation = GetActorLocation();
	//FVector parentLocation = TailActorParent->GetActorLocation();

	//float lenge = FVector::Distance(tailLocation, parentLocation);

	//// êKîˆÇÃìÆÇ´ÇÃêßå¿
	//if (lenge >= tailPosLimit) { return; }

	//// èdóÕÇâ¡Ç¶ÇÈ
	//AddVec += GravityScale;

	//// óÕÇâ¡Ç¶ÇÈ
	//if (lenge >= tailAddPysr) {

	//	float Diagonal = FVector2D(x,y).Size();
	//	float z = sqrt(1 - Diagonal * Diagonal);

	//	FVector StandardVec =
	//		FVector(
	//			x,//x
	//			y,//y
	//			z //z
	//		);		
	//}

	float Diagonal = FVector2D(x, y).Size();
	float z = sqrt(1 - Diagonal * Diagonal);

	FVector StandardVec =
		FVector(
			x,//x
			y,//y
			z //z
		);

	FVector actorVec = GetActorLocation();
	SetActorLocation(actorVec + StandardVec * 10);
}

void ATailActor::SetupAttachment(USceneComponent* sc_) {

	TailMesh->SetupAttachment(sc_);
}

