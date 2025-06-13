// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall/FireBall_ver1.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireBall_ver1::AFireBall_ver1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMeshComponent��ǉ����ARootComponent�ɐݒ肷��
	FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = FireBall;

	// StaticMesh��Laod����StaticMeshComponent��StaticMesh�ɐݒ肷��
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

	// StaticMesh��StaticMeshComponent�ɐݒ肷��
	FireBall->SetStaticMesh(Mesh);

	//�X�t�B�A�R���W�����쐬
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("BlockAll");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(20.0f);

	// Material��StaticMesh�ɐݒ肷��
	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	// Material��StaticMeshComponent�ɐݒ肷��
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

	
	//�����őO�ɐi��
	FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * 10000.f * DeltaTime);

	SetActorLocation(NewLocation);
}

void AFireBall_ver1::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {

	// pass
}

