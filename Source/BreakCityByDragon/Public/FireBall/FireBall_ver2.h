#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "FireBall_ver2.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

class USoundBase;
class USoundAttenuation;
class UNiagaraSystem;

UCLASS()
class BREAKCITYBYDRAGON_API AFireBall_ver2 : public AActor
{
    GENERATED_BODY()

public:
    AFireBall_ver2();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // --- ?件区 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> FireBall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USphereComponent> SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> TrailComponent;

    // --- 属性区 ---
    // ?行?可被??的材?基底，用于?建???例，避免引用 EditorOnlyData
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Effects")
    TObjectPtr<UMaterialInterface> BaseMaterialAsset;

    // ??器中指定的?尾特效??
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Effects")
    TObjectPtr<UNiagaraSystem> TrailEffectAsset;

    //当たるエフェクト
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireBall|Effects")
    TArray<UNiagaraSystem*> HitNiagaraEffects;

    //効果音（attack)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Audio")
    TObjectPtr<USoundBase> HitSFX = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Audio")
    TObjectPtr<USoundAttenuation> HitAttenuation;

    //効果音(shoot)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireBall|Audio")
    USoundBase* LaunchSFX;

    // --- 函数区 ---
    UFUNCTION()
    void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private :

    bool DestroyFlag;
    float timeCnt;

    //効果音一回だけプレイ
    bool bHitSFXPlayed = false;

    // 定数

    const float FollowSpeed     = 3000.0f;
    const float MaxDestroyCnt   = 0.1f;
};