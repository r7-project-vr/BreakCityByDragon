// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageText.h"

int32 UDamageText::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	UE_LOG(LogTemp, Warning, TEXT("Widget Size: %s"), *AllottedGeometry.GetLocalSize().ToString());

	// 矩形描画
	//{
	//	LayerId++;
	//	FVector2D Position = AllottedGeometry.GetLocalSize() * 0.5f;
	//	FVector2D Offset = FVector2D(0.0f, -50.0f * (1.0f - CurrentAlpha));
	//	FVector2D DrawPos = Position + Offset;
	//	FVector2D Size = Brush.GetImageSize() * 10;
	//	FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(DrawPos, Size), &Brush);

	//	return FMath::Max(LayerId, LayerId);
	//}

	UE_LOG(LogTemp, Warning, TEXT("Painting Test Text"));

	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(FVector2D(0, 0), FVector2D::ZeroVector),
		FText::FromString(TEXT("HELLO")),
		FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 32),
		ESlateDrawEffect::None,
		FLinearColor::Yellow
	);

	UE_LOG(LogTemp, Warning, TEXT("Size: %s"), *AllottedGeometry.GetLocalSize().ToString());
	UE_LOG(LogTemp, Warning, TEXT("NativePaint called!"));
	return LayerId + 1;

	//// 文字の処理
	//{
	//	const ESlateDrawEffect DrawEffects = bParentEnabled
	//		? ESlateDrawEffect::None
	//		: ESlateDrawEffect::DisabledEffect;

	//	// 描画位置の調整（中央に表示）
	//	FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
	//	FVector2D TextPosition = FVector2D(0.0f, 0.0f); // デバッグ用

	//	// 色と透明度の調整
	//	FLinearColor Tint = InWidgetStyle.GetColorAndOpacityTint();
	//	Tint.A *= CurrentAlpha;

	//	FString TextString = FString::Printf(TEXT("%d"), DamageValue);
	//	FText DamageText = FText::FromString(TextString);

	//	// 実際のテキスト描画
	//	FSlateDrawElement::MakeText(
	//		OutDrawElements,
	//		LayerId,
	//		AllottedGeometry.ToPaintGeometry(TextPosition, FVector2D::ZeroVector),
	//		DamageText,
	//		FontInfo,
	//		DrawEffects,
	//		Tint
	//	);

	//	return LayerId + 1;
	//}
}

void UDamageText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {

	Super::NativeTick(MyGeometry, InDeltaTime);

	// 透明度を減らす（1秒で消える）
	//CurrentAlpha -= InDeltaTime;
	CurrentAlpha = FMath::Clamp(CurrentAlpha, 0.0f, 1.0f);

	UE_LOG(LogTemp, Warning, TEXT("GoText"));

	if (CurrentAlpha <= 0.0f)
	{
		RemoveFromParent();
	}
}