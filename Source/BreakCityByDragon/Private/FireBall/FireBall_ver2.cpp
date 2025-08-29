#include "FireBall/FireBall_ver2.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

AFireBall_ver2::AFireBall_ver2()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. ‰Î‹…–ÍŒ^?Œ
    FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = FireBall;

    // 2. ?“³?Œ
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    SphereComponent->SetSphereRadius(30.0f);

    // 3. ??Ş??—áiİ BeginPlay ??ŒšC?•Û BaseMaterialAsset ›ß??j

    // 4. ?”ö?Œ
    TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailComponent"));
    TrailComponent->SetupAttachment(RootComponent);

    // 5. ?’è?“³–Œ
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireBall_ver2::OnHit);
}

void AFireBall_ver2::BeginPlay()
{
    Super::BeginPlay();

    // ?Œš›ó?—p??Ş??—á
    if (BaseMaterialAsset && FireBall)
    {
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterialAsset, this);
        if (DynMat)
        {
            DynMat->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
            FireBall->SetMaterial(0, DynMat);
        }
    }

    // ŒƒŠˆ?”ö“ÁÁ
    if (TrailEffectAsset && TrailComponent)
    {
        TrailComponent->SetAsset(TrailEffectAsset);
        TrailComponent->Activate(true);
    }
}

void AFireBall_ver2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * 1000.f * DeltaTime);
    SetActorLocation(NewLocation);
}

void AFireBall_ver2::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == this) return;
    //if (!OtherActor || OtherActor == this) return;

    /*if (TrailComponent)
    {
        TrailComponent->Deactivate();
    }
    FireBall->SetVisibility(false);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PrimaryActorTick.bCanEverTick = false;
    SetLifeSpan(2.0f);
    // TODO: “Y‰Á?ŠQ˜a‰¹Á??*/

    UKismetSystemLibrary::PrintString(GEngine->GetWorld(), OtherActor->GetName());
    this->Destroy();
}
