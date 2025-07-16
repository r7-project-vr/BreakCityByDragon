// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h" // 新增：为UNiagaraSystem资源提供完整定义
#include "FireBall_ver2.generated.h"

// 向前声明，避免在头文件中引入过重的依赖
class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

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

	// 用于显示火球模型的组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> FireBall;

	// 用于处理碰撞的球体组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

	// 新增：用于显示拖尾效果的Niagara组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> TrailComponent;

	// --- 属性区 ---

	// 新增：暴露给蓝图的属性，用于在编辑器中设置拖尾特效资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBall|Effects")
	TObjectPtr<UNiagaraSystem> TrailEffectAsset;


	// --- 函数区 ---

	// 用于处理碰撞事件的函数
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};