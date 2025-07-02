// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ATailActor::ATailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("rootComponent"));
	RootComponent = root;

	TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
	UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	TailMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	TailMesh->SetStaticMesh(Sphere);
	TailMesh->SetupAttachment(RootComponent);
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

void ATailActor::TailMove(FVector* vec) {

	float r = 1.0f;
	FVector tailLocation = GetActorLocation();
	FVector parentLocation = TailActorParent->GetActorLocation();


}

