// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/TailActor_ver2.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/SoftObjectPtr.h"

// Sets default values
ATailActor_ver2::ATailActor_ver2() :
    TailInstance(nullptr),
    FirstRotateSetFlag(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // コンポーネント作成＆ルートに設定
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);

    // アセットパス指定（プロジェクトに合わせて変更）
    SoftSkeletalMeshRef = TSoftObjectPtr<USkeletalMesh>(
        FSoftObjectPath(TEXT("/Game/Dradon/Dragon_1/TailSkeleton.TailSkeleton"))
    );


    // 角度の初期化
    {
        int size = sizeof(DeviceRotate) / sizeof(DeviceRotate[0]);

        for (int n = 0; n < size; n++) {

            DeviceRotate[n] = FRotator::ZeroRotator;
            FirstRotate[n] = FRotator(10000, 0, 0);//初期値はでたらめ
        }
    }
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

    // pass
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
        TSoftClassPtr<UAnimInstance>(FSoftObjectPath(TEXT("/Game/Dradon/Dragon_1/TailAnimation.TailAnimation_C")));

    AnimBPClassRef.LoadSynchronous();

    if (AnimBPClassRef.IsValid())
    {
        SkeletalMeshComponent->SetAnimInstanceClass(AnimBPClassRef.Get());
        UAnimInstance* AnimInst = SkeletalMeshComponent->GetAnimInstance();
        if (UTailAnimInstance* TI = Cast<UTailAnimInstance>(AnimInst)) {

            TailInstance = TI;
            UE_LOG(LogTemp, Error, TEXT("AnimInstance successfully loaded!"));
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
    if (!TailInstance) return;

    for (int n = 0; n < 3; n++) {

        FRotator NewRotation = DeviceRotate[n] - FirstRotate[n];

        // 再調整
        SetRotation(NewRotation);

        TailInstance->TailBoneRotation_Senser[n] = NewRotation;

        FString s = TailInstance->TailBoneRotation_Senser[n].ToString();
        UE_LOG(LogTemp, Log, TEXT("TailBoneRotation_Senser%d : %s "), n, *s);
    }
}

bool ATailActor_ver2::ResetRotation(FRotator* r, int i_) {

    bool set = false;

    int size = sizeof(FirstRotate) / sizeof(FirstRotate[0]);
    if (i_ != size) { return set; }

    for (int n = 0; n < i_; n++) {

        FirstRotate[n] = r[n];
        FString s = FirstRotate[n].ToString();
        UE_LOG(LogTemp, Log, TEXT("FirstRotate%d : %s "), n, *s);
    }

    int index = 0;

    while(1) {

        if (index >= size) { 

            set = true;
            break;
        }

        if (FirstRotate[index].Pitch == 10000) {

            break;
        }

        index++;
    }

    return set;
}

void ATailActor_ver2::SetDeviceRotate(FRotator* r,int size) {

    int RotateSize = sizeof(DeviceRotate) / sizeof(DeviceRotate[0]);

    if (size != RotateSize) { return; }

    for (int n = 0; n < RotateSize; n++)
        DeviceRotate[n] = r[n];

    UpdateTailRotation();
}

void ATailActor_ver2::SetRotation(FRotator& r) {

    // Yaw

    r.Yaw = 0;

    // Pitch 

    if (0 < r.Pitch && r.Pitch < 90) {
    
        r.Pitch *= PitchProduct;
    }
    else if (r.Pitch > 270) {

        float f = 360.f - r.Pitch;

        f *= PitchProduct;

        r.Pitch = 360.f - f;
    }


    // Roll 

    if (0 < r.Roll && r.Roll < 90) {

        r.Roll *= RollProduct;
    }
    else if (r.Roll > 270) {

        float f = 360.f - r.Roll;

        f *= RollProduct;

        r.Roll = 360.f - f;
    }
}
