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

    // --- 组件区 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> FireBall;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USphereComponent> SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> TrailComponent;

    // --- 属性区 ---
    // 运行时可被编辑的材质基底，用于创建动态实例，避免引用 EditorOnlyData
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Effects")
    TObjectPtr<UMaterialInterface> BaseMaterialAsset;

    // 编辑器中指定的拖尾特效资产
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Effects")
    TObjectPtr<UNiagaraSystem> TrailEffectAsset;

    // --- 函数区 ---
    UFUNCTION()
    void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};