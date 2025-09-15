#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorSimpleActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class BREAKCITYBYDRAGON_API ADoorSimpleActor : public AActor
{
    GENERATED_BODY()

public:
    ADoorSimpleActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UFUNCTION()
    void OnProximityBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private:
    /** door mesh */
    UPROPERTY(VisibleAnywhere, Category = "Door")
    UStaticMeshComponent* DoorMesh;

    /** collision */
    UPROPERTY(VisibleAnywhere, Category = "Door")
    UBoxComponent* Proximity;

    /** 上がる高さ */
    UPROPERTY(EditAnywhere, Category = "Door|Config")
    float OpenHeight = 2500.f;

    /** 移動速度 */
    UPROPERTY(EditAnywhere, Category = "Door|Config")
    float OpenSpeed = 300.f;

    /** プレイヤー０にだけ反応する */
    UPROPERTY(EditAnywhere, Category = "Door|Config")
    bool bOnlyPlayer0 = true;

    /** 元位置 */
    FVector StartRelLoc;

    /** 目標位置 */
    FVector TargetOpenRelLoc;

    /** 開けてる */
    bool bOpening = false;

    /** 開けた（もう閉めない） */
    bool bOpened = false;
};
