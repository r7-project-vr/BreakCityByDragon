#include "TitleDoorOpen.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


ATitleDoorOpen::ATitleDoorOpen() 
{    

    PrimaryActorTick.bCanEverTick = true;

    
    bIsRising = false;
    bHasRisen = false;
    RiseSpeed = 100.0f; 
    TargetRelativeLocation = FVector(0.f, 0.f, 200.f); 

    
    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootComp;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);

    
    DoorMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    
    DoorMesh->SetUseCCD(true);

    
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    TriggerVolume->SetupAttachment(RootComponent);

    
    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}


void ATitleDoorOpen::BeginPlay()
{
    Super::BeginPlay();

    
    InitialRelativeLocation = DoorMesh->GetRelativeLocation();

    
    if (TriggerVolume)
    {
        
        TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ATitleDoorOpen::OnTriggerBeginOverlap);
    }
}


void ATitleDoorOpen::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    
    if (bIsRising)
    {
        FVector FinalTargetLocation = InitialRelativeLocation + TargetRelativeLocation;

        FVector CurrentLocation = DoorMesh->GetRelativeLocation();

        FVector NewLocation = FMath::VInterpConstantTo(
            CurrentLocation,     
            FinalTargetLocation, 
            DeltaTime,           
            RiseSpeed            
        );

        DoorMesh->SetRelativeLocation(NewLocation);

        if (FVector::DistSquared(NewLocation, FinalTargetLocation) < KINDA_SMALL_NUMBER)
        {
            DoorMesh->SetRelativeLocation(FinalTargetLocation);

            bIsRising = false;

            bHasRisen = true;

        }
    }
}

// 
void ATitleDoorOpen::OnTriggerBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || bHasRisen || bIsRising)
    {
        return;
    }

    
    APawn* OverlappingPawn = Cast<APawn>(OtherActor);

    
    if (OverlappingPawn && OverlappingPawn->IsPlayerControlled())
    {
        
        bIsRising = true;
    }
}