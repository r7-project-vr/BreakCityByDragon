// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ATailActor::ATailActor() :
	MoveVec(0.f, 0.f, 0.f),
	PreParentLocation(-1000000, 0, 0)
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
		if (TailActorParent == nullptr) {
			return;
		}
	}

	{
		FVector rig = TailActorParent->GetParentMoveVector();
		TailMove(rig);
	}
}

void ATailActor::SetParentTail(AActor* tail) {

	TailActorParent = Cast<IITailInformaition>(tail);
}

void ATailActor::TailMove(FVector rig){

	if (PreParentLocation == FVector(-1000000,0,0)) {

		PreParentLocation = TailActorParent->GetParentActorLocation();
		return;
	}

	// 現在のアクターの位置を保存する
	FVector actorVec = GetActorLocation();

	// 計算した移動距離を保存する変数
	MoveVec = FVector::ZeroVector;// いったんゼロ
	FVector chaceVec = FVector::ZeroVector;

	// 親のアクターから一定の距離離れたら力を加える
	{
		FVector pVec = TailActorParent->GetParentActorLocation();
		float distance = FVector::DistSquared(actorVec, pVec);

		if (distance > maxTailLenge) {// 親のアクターから一定の距離離れてるとき
			
			chaceVec = PreParentLocation - pVec;
		}
	}
	
	// 親アクターに加わっている力に合わせた移動距離を計算する
	{
		MoveVec = {
			rig.X * adjustPow,
			rig.Y * adjustPow,
			rig.Z * adjustPow
		};
	}

	FVector addVec = actorVec + chaceVec;

	

	// アクターの角度を移動した距離に応じてそろえる
	{

	}

	// アクターの位置の移動
	SetActorLocation(addVec);

	PreParentLocation = TailActorParent->GetParentActorLocation();
}

FVector ATailActor::GetParentActorLocation() {

	return this->GetActorLocation();
}

FVector ATailActor::GetParentMoveVector() {

	return MoveVec * 2;
}