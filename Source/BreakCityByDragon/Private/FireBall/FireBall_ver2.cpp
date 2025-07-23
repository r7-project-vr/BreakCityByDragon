#include "FireBall/FireBall_ver2.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"
#include "UObject/ConstructorHelpers.h"

AFireBall_ver2::AFireBall_ver2()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. 火球模型组件
    FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = FireBall;

    // 2. 碰撞组件
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
    SphereComponent->SetSphereRadius(30.0f);

    // 3. 动态材质实例（在 BeginPlay 时创建，确保 BaseMaterialAsset 已赋值）

    // 4. 拖尾组件
    TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailComponent"));
    TrailComponent->SetupAttachment(RootComponent);

    // 5. 绑定碰撞事件
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireBall_ver2::OnHit);
}

void AFireBall_ver2::BeginPlay()
{
    Super::BeginPlay();

    // 创建并应用动态材质实例
    if (BaseMaterialAsset && FireBall)
    {
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterialAsset, this);
        if (DynMat)
        {
            DynMat->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
            FireBall->SetMaterial(0, DynMat);
        }
    }

    // 激活拖尾特效
    if (TrailEffectAsset && TrailComponent)
    {
        TrailComponent->SetAsset(TrailEffectAsset);
        TrailComponent->Activate(true);
    }
}

void AFireBall_ver2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * 6000.f * DeltaTime);
    SetActorLocation(NewLocation);
}

void AFireBall_ver2::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    if (TrailComponent)
    {
        TrailComponent->Deactivate();
    }
    FireBall->SetVisibility(false);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PrimaryActorTick.bCanEverTick = false;
    SetLifeSpan(2.0f);
    // TODO: 添加伤害和音效逻辑
}
