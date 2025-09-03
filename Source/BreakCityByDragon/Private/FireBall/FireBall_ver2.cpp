#include "FireBall/FireBall_ver2.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"

AFireBall_ver2::AFireBall_ver2():
    DestroyFlag(false),
    timeCnt(0),
    bHitSFXPlayed(false)
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. ファイヤーボールモデルコンポネント
    FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = FireBall;

    // 2. collisionコンポネント
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    SphereComponent->SetSphereRadius(100.0f);

    // 3. ??材??例（在 BeginPlay ??建，?保 BaseMaterialAsset 已??）

    // 4. トレーンコンポネント
    TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailComponent"));
    TrailComponent->SetupAttachment(RootComponent);

    // 5. ?定?撞事件
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireBall_ver2::OnHit);
}

void AFireBall_ver2::BeginPlay()
{
    Super::BeginPlay();

    if (HitSFX) { UGameplayStatics::PlaySound2D(this, HitSFX); }//test音が出れるか否か
    // ?建并?用??材??例
    if (BaseMaterialAsset && FireBall)
    {
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterialAsset, this);
        if (DynMat)
        {
            DynMat->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
            FireBall->SetMaterial(0, DynMat);
        }
    }

    // 激活?尾特效
    if (TrailEffectAsset && TrailComponent)
    {
        TrailComponent->SetAsset(TrailEffectAsset);
        TrailComponent->Activate(true);
    }
}

void AFireBall_ver2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * FollowSpeed * DeltaTime);
    SetActorLocation(NewLocation);

    if (DestroyFlag) {

        timeCnt += DeltaTime;

        if (timeCnt >= MaxDestroyCnt) {

            this->Destroy();
        }
    }
}

void AFireBall_ver2::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    UKismetSystemLibrary::PrintString(GetWorld(),
        TEXT("OnHit fired!"), true, true, FLinearColor::Green, 1.5f);//test
    if (OtherActor == this) return;
    //if (!OtherActor || OtherActor == this) return;

    //if (TrailComponent)
    //{
    //    TrailComponent->Deactivate();
    //}
    //FireBall->SetVisibility(false);
    //SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //PrimaryActorTick.bCanEverTick = false;
    //SetLifeSpan(2.0f);
    //// TODO: 添加?害和音效??

    //Attackの時に効果音一回プレイ
    if (!bHitSFXPlayed && HitSFX) {
        UGameplayStatics::PlaySoundAtLocation(
            this,                       // WorldContext
            HitSFX,                     // AudioFile
            GetActorLocation(),         // PlayLocation
            1.0f,                       // Volume
            1.0f,                       // Pitch
            0.0f,                       //StartTime
            HitAttenuation
        );
        bHitSFXPlayed = true;
    }
    /*if (HitSFX)
    {
        UGameplayStatics::PlaySound2D(this, HitSFX, 1.0f, 1.0f, 0.0f);
        UKismetSystemLibrary::PrintString(GetWorld(),
            TEXT("HitSFX PlaySound2D called"), true, true, FLinearColor::Yellow, 1.5f);
    }
    else
    {
        UKismetSystemLibrary::PrintString(GetWorld(),
            TEXT("HitSFX is NOT set on the Blueprint"), true, true, FLinearColor::Red, 2.0f);
    }*/

    DestroyFlag = true;
    UE_LOG(LogTemp, Warning, TEXT("Hit"));
    /*this->Destroy();*/
}
