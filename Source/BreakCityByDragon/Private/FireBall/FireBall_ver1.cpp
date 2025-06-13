// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall/FireBall_ver1.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireBall_ver1::AFireBall_ver1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMeshComponentを追加し、RootComponentに設定する
	FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = FireBall;

	// StaticMeshをLaodしてStaticMeshComponentのStaticMeshに設定する
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

	// StaticMeshをStaticMeshComponentに設定する
	FireBall->SetStaticMesh(Mesh);

	//スフィアコリジョン作成
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("BlockAll");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(20.0f);

	// MaterialをStaticMeshに設定する
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	// MaterialをStaticMeshComponentに設定する
	FireBall->SetMaterial(0, Material);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireBall_ver1::OnHit);

}

// Called when the game starts or when spawned
void AFireBall_ver1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireBall_ver1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//自動で前に進む
	FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * 10000.f * DeltaTime);

	SetActorLocation(NewLocation);
}

void AFireBall_ver1::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {

	// pass
}

