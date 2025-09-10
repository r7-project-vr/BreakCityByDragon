// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor_ver2.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/SoftObjectPtr.h"

// Sets default values
ATailActor_ver2::ATailActor_ver2() :
    SerialReceiver(nullptr),
    TailInstance(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // コンポーネント作成＆ルートに設定
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);

    // アセットパス指定（プロジェクトに合わせて変更）
    SoftSkeletalMeshRef = TSoftObjectPtr<USkeletalMesh>(
        FSoftObjectPath(TEXT("/Game/Dradon/TailSkeleton.TailSkeleton"))
    );
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
        UAnimInstance* AnimInst = SkeletalMeshComponent->GetAnimInstance();
        if (UTailAnimInstance* TI = Cast<UTailAnimInstance>(AnimInst)) {

            TailInstance = TI;
        }
        else { 
            UE_LOG(LogTemp, Error, TEXT("No AnimInstance"));
        }
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
    if (!TailInstance)return;
    if (!SerialReceiver)return;

    TailInstance->TailBoneRotation_Senser1 = SerialReceiver->GetRotation(1);
    TailInstance->TailBoneRotation_Senser2 = SerialReceiver->GetRotation(2);
    TailInstance->TailBoneRotation_Senser3 = SerialReceiver->GetRotation(3);
}

