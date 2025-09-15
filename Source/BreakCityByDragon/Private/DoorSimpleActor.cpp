#include "DoorSimpleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ADoorSimpleActor::ADoorSimpleActor()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false); // 初始不Tick

    // door mesh
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    SetRootComponent(DoorMesh);
    DoorMesh->SetMobility(EComponentMobility::Movable);
    DoorMesh->SetCollisionProfileName(TEXT("BlockAll"));

    // trigger box
    Proximity = CreateDefaultSubobject<UBoxComponent>(TEXT("Proximity"));
    Proximity->SetupAttachment(RootComponent);
    Proximity->SetBoxExtent(FVector(200.f, 200.f, 200.f));
    Proximity->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Proximity->SetCollisionResponseToAllChannels(ECR_Ignore);
    Proximity->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Proximity->SetGenerateOverlapEvents(true);

    Proximity->OnComponentBeginOverlap.AddDynamic(this, &ADoorSimpleActor::OnProximityBegin);
}

void ADoorSimpleActor::BeginPlay()
{
    Super::BeginPlay();

    StartRelLoc = DoorMesh->GetRelativeLocation();
    TargetOpenRelLoc = StartRelLoc + FVector(0.f, 0.f, OpenHeight);
}

void ADoorSimpleActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bOpening) return;

    const FVector Current = DoorMesh->GetRelativeLocation();
    const FVector NewLoc = FMath::VInterpConstantTo(Current, TargetOpenRelLoc, DeltaSeconds, OpenSpeed);
    DoorMesh->SetRelativeLocation(NewLoc);

    if (NewLoc.Equals(TargetOpenRelLoc, 0.1f))
    {
        bOpening = false;
        SetActorTickEnabled(false); // 开到位就停Tick
    }
}

void ADoorSimpleActor::OnProximityBegin(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor,
    UPrimitiveComponent* /*OtherComp*/, int32 /*OtherBodyIndex*/,
    bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
    if (bOpened || !OtherActor) return;

    if (bOnlyPlayer0)
    {
        if (OtherActor != UGameplayStatics::GetPlayerPawn(this, 0)) return;
    }

    bOpened = true;     // 一回だけ開ける
    bOpening = true;    // 開け始める
    SetActorTickEnabled(true);
}
