// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TestTail.h"

// Sets default values
ATestTail::ATestTail():
	Radians(0),
	cnt(0)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TestObj= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestTail"));

	UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	TestObj->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	TestObj->SetStaticMesh(Sphere);

	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Material, this);
	if (DynMat)
	{
		DynMat->SetVectorParameterValue(FName("Color"), FLinearColor::Red);
		TestObj->SetMaterial(0, DynMat);
	}
	else {
		TestObj->SetMaterial(0, Material);
	}
	

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;
	TestObj->SetupAttachment(RootComponent);

	for (ATailActor* tails:tail) {
		tails = nullptr;
	}

	// Arrowの初期化
	{
		// Arrowを追加する
		Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
		Arrow->SetupAttachment(RootComponent);

		Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		// Arrowを表示されるようにする
		Arrow->bHiddenInGame = false;
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
			FVector pos = GetActorLocation() + FVector(-20.f, 0, 0);

			tail[0] = GetWorld()->SpawnActor<ATailActor>(ATailActor::StaticClass(), pos, look);
			tail[0]->SetParentTail(this);
		}

		for (int i = 1;i < 6;i++) {

			// しっぽの位置
			FVector pos = GetActorLocation() + FVector(-20.f, 0, 0) * (i + 1);
			tail[i] = GetWorld()->SpawnActor<ATailActor>(ATailActor::StaticClass(), pos, look);
			tail[i]->SetParentTail(tail[i - 1]);
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
FVector ATestTail::GetTargetActorLocation() {

	return GetActorLocation();
}

// しっぽの角度を返す
FRotator ATestTail::GetTargetActorRotation() {

	return GetActorRotation();
}

// アクターの前方方向のベクトルを返す
FVector ATestTail::GetTargetActorForwardVector() {

	return GetActorForwardVector();
}

void ATestTail::moveVec() {

	// 移動処理
	{
		FVector addVec = FVector::Zero();

		int index = Radians % 4;

		switch (index)
		{
		case 0:
			addVec = FVector(3.0f, 0, 0);
			break;
		case 1:
			addVec = FVector(0, 3.0f, 0);
			break;
		case 2:
			addVec = FVector(-3.0f, 0, 0);
			break;
		case 3:
			addVec = FVector(0, -3.0f, 0);
			break;
		default:
			break;
		}

		FVector nowVec = GetActorLocation();
		FVector newVec = nowVec + addVec + DeviceVec + FVector(2.0f, 0, 0);

		newVec = GetActorForwardVector() + GetActorLocation();

		SetActorLocation(newVec);

		cnt++;

		if (cnt >= 10) {

			cnt = 0;
			if (Radians % 2 == 1) { DeviceVec = FVector(0, 0, 5); }
			else { DeviceVec = FVector(0, 0, -5); }
			Radians++;
		}
	}
	
	// 回転処理
	{
		FRotator NewRoTation = GetActorRotation();
		
		NewRoTation.Yaw += 90.f / 30;

		SetActorRotation(NewRoTation);

		FVector MyVector = GetActorForwardVector();

		UE_LOG(LogTemp, Warning, TEXT("MyVector: %s"), *MyVector.ToString());
	}
}