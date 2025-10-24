#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleDoorOpen.generated.h" 

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class BREAKCITYBYDRAGON_API ATitleDoorOpen : public AActor 
{
    GENERATED_BODY()

public:
    
    ATitleDoorOpen(); 

protected:
   
    virtual void BeginPlay() override;

public:
    
    virtual void Tick(float DeltaTime) override;

   
    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootComp;

    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* DoorMesh;

    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerVolume;

   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector TargetRelativeLocation;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RiseSpeed; 

private:
    
    FVector InitialRelativeLocation;

    
    bool bIsRising; 

    
    bool bHasRisen; 

   
    UFUNCTION()
    void OnTriggerBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
