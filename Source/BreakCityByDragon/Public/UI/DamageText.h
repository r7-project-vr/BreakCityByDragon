// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageText.generated.h"

/**
 *
 */
UCLASS()
class BREAKCITYBYDRAGON_API UDamageText : public UUserWidget
{
	GENERATED_BODY()

		virtual int32 NativePaint(
			const FPaintArgs& Args,
			const FGeometry& AllottedGeometry,
			const FSlateRect& MyCullingRect,
			FSlateWindowElementList& OutDrawElements,
			int32 LayerId,
			const FWidgetStyle& InWidgetStyle,
			bool bParentEnabled) const override;
	
protected:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

private:

	// 画像(ブループリントで編集)
	UPROPERTY(EditAnywhere)
	FSlateBrush Brush;

	// ダメージ数
	UPROPERTY(EditAnywhere, Category = "DamageValue")
	int DamageValue = 100;

	// フォント
	//UPROPERTY(EditAnywhere,Category="TextFont")
	FSlateFontInfo FontInfo = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24);

	// 定数

	float CurrentAlpha = 1.0f; // 透明度（0?1）

	FVector2D BasePosition; // 元の位置
};
