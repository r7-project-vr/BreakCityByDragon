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
#include "NiagaraFunctionLibrary.h"

AFireBall_ver2::AFireBall_ver2():
    DestroyFlag(false),
    timeCnt(0),
    bHitSFXPlayed(false),
    defaltTimeCnt(0)
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

    //発射の効果音
    if (LaunchSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(this, LaunchSFX, GetActorLocation());
    }

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

    defaltTimeCnt += DeltaTime;

    if (defaltTimeCnt >= MaxDefaltDestroyCnt) {

        this->Destroy();
    }

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
    
    if (OtherActor == this) return;
    if (DestroyFlag) return;

    //Attackの時に効果音一回プレイ
    if (!bHitSFXPlayed && HitSFX) {
        UKismetSystemLibrary::PrintString(GetWorld(),
            TEXT("OnHit fired!"), true, true, FLinearColor::Green, 1.5f);//test
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

     /*--- Attack所で爆発のエフェクトをプレイ ---
     1. チェックエフェクトArrayはNullかどうか*/
    if (HitNiagaraEffects.Num() > 0)
    {
        // 2. 0 から配列の末尾までの範囲でランダムなインデックスを生成する
        const int32 RandomIndex = FMath::RandRange(0, HitNiagaraEffects.Num() - 1);

        // 3. ランダムなインデックスを使って配列からNiagaraシステムを取得する
        TWeakObjectPtr<UNiagaraSystem> SelectedEffect = HitNiagaraEffects[RandomIndex];

        // 4. 選択したエフェクトが有効（nullptr ではない）であることを確認する
        if (SelectedEffect.IsValid())
        {
            FVector ImpactLocation;
            // 衝突結果（SweepResult）から正確なヒット位置と法線方向を取得する
            //const FVector ImpactLocation = SweepResult.ImpactPoint;
            OtherComp->GetClosestPointOnCollision(GetActorLocation(), ImpactLocation);
            //const FRotator ImpactRotation = SweepResult.ImpactNormal.Rotation();
            const FRotator ImpactRotation = (GetActorLocation() - ImpactLocation).Rotation();

            // 5. ヒット地点に Niagara エフェクトを生成する
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                SelectedEffect.Get(),
                ImpactLocation,
                ImpactRotation
            );
        }
    }

    DestroyFlag = true;
    UE_LOG(LogTemp, Warning, TEXT("Hit"));
}
