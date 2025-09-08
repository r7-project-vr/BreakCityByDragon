// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor_ver2.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/SoftObjectPtr.h"
#include "VRDragon/TailAnimInstance.h"

// Sets default values
ATailActor_ver2::ATailActor_ver2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // コンポーネント作成＆ルートに設定
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    RootComponent = SkeletalMeshComponent;

    // アセットパス指定（プロジェクトに合わせて変更）
    SoftSkeletalMeshRef = TSoftObjectPtr<USkeletalMesh>(
        FSoftObjectPath(TEXT("/Game/Dradon/TailSkeleton.TailSkeleton"))
    );

    //SkeletalMeshComponent->SetAnimInstanceClass(MyAnimBPClass);
}

// Called when the game starts or when spawned
void ATailActor_ver2::BeginPlay()
{
	Super::BeginPlay();

    LoadMeshAsync();
    LoadAnimBPAsync();
}

// Called every frame
void ATailActor_ver2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateTailRotation();
}

void ATailActor_ver2::LoadMeshAsync() {

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    if (SoftSkeletalMeshRef.IsValid())
    {
        // 既にロード済みなら即座に反映
        OnMeshLoaded();
    }
    else
    {
        // 非同期ロード開始
        Streamable.RequestAsyncLoad(
            SoftSkeletalMeshRef.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &ATailActor_ver2::OnMeshLoaded)
        );
    }
}

void ATailActor_ver2::LoadAnimBPAsync() {

    TSoftClassPtr<UAnimInstance> AnimBPClassRef = 
        TSoftClassPtr<UAnimInstance>(FSoftObjectPath(TEXT("/Game/Dradon/TailAnimation.TailAnimation_C")));

    AnimBPClassRef.LoadSynchronous();

    if (AnimBPClassRef.IsValid())
    {
        SkeletalMeshComponent->SetAnimInstanceClass(AnimBPClassRef.Get());
    }
}

void ATailActor_ver2::OnMeshLoaded() {

    USkeletalMesh* LoadedMesh = SoftSkeletalMeshRef.Get();

    if (LoadedMesh)
    {
        SkeletalMeshComponent->SetSkeletalMesh(LoadedMesh);
        SkeletalMeshComponent->SetWorldScale3D(FVector(50.0f));
        UE_LOG(LogTemp, Log, TEXT("SkeletalMesh successfully loaded!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load SkeletalMesh."));
    }
}

void ATailActor_ver2::UpdateTailRotation() {

    if (!SkeletalMeshComponent) return;

    UAnimInstance* AnimInst = SkeletalMeshComponent->GetAnimInstance();
    if (UTailAnimInstance* TailInst = Cast<UTailAnimInstance>(AnimInst))
    {
        TailInst->TailBoneRotation_Senser1 = FRotator(20.f, 0.f, 0.f);
        UE_LOG(LogTemp, Log, TEXT("SkeletalMesh Anim"));
    }
}

