// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageText.h"
#include "Widgets/SWidget.h"

int32 UDamageText::NativePaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
		const ESlateDrawEffect DrawEffects = bParentEnabled
		? ESlateDrawEffect::None
		: ESlateDrawEffect::DisabledEffect;

	// 描画ジオメトリ（位置 + サイズ）
	const FPaintGeometry PaintGeometry = AllottedGeometry.ToPaintGeometry();

	// 表示するテキスト
	const FString TextString = FString::Printf(TEXT("%d"), DamageValue);
	const FText DamageText = FText::FromString(TextString);

	// フォント情報の設定（ここでは CoreStyle のデフォルトフォントを使う）
	const FSlateFontInfo _FontInfo = FCoreStyle::Get().GetFontStyle("Regular");

	// テキスト色
	const FLinearColor TextColor = FLinearColor::White;

	// テキスト描画命令を追加
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,                   // 現在のレイヤー
		PaintGeometry,
		DamageText,
		_FontInfo,
		DrawEffects,
		TextColor
	);

	// 他の描画処理を行う場合は LayerId をインクリメントして返す
	return LayerId + 1;
}

void UDamageText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {

	Super::NativeTick(MyGeometry, InDeltaTime);

	// 透明度を減らす（1秒で消える）
	CurrentAlpha -= InDeltaTime;
	CurrentAlpha = FMath::Clamp(CurrentAlpha, 0.0f, 1.0f);

	UE_LOG(LogTemp, Warning, TEXT("GoText"));

	if (CurrentAlpha <= 0.0f)
	{
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("Delete Text"));
	}
}