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

    // --- 【新增】在撞击点播放一个随机的Niagara特效 ---
    // 1. 检查特效数组是否为空
    if (HitNiagaraEffects.Num() > 0)
    {
        // 2. 从0到数组末尾之间，生成一个随机的索引
        const int32 RandomIndex = FMath::RandRange(0, HitNiagaraEffects.Num() - 1);

        // 3. 使用随机索引从数组中获取一个Niagara系统
        UNiagaraSystem* SelectedEffect = HitNiagaraEffects[RandomIndex];

        // 4. 确保选中的特效是有效的（不是空指针）
        if (SelectedEffect)
        {
            // 从碰撞结果(SweepResult)中获取精确的撞击点位置和法线方向
            const FVector ImpactLocation = SweepResult.ImpactPoint;
            const FRotator ImpactRotation = SweepResult.ImpactNormal.Rotation();

            // 5. 在撞击点生成Niagara特效
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                SelectedEffect,
                ImpactLocation,
                ImpactRotation
            );
        }
    }
    

    DestroyFlag = true;
    UE_LOG(LogTemp, Warning, TEXT("Hit"));
    /*this->Destroy();*/
}
