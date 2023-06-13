#include "MCOHpWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UMCOHpWidget::UMCOHpWidget(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	CurrentHp = -1.0f;
}

void UMCOHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtName")));
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));

	ensure(NameText);
	ensure(HpProgressBar);
}

void UMCOHpWidget::SetName(const FString& InName)
{
	ISTRUE(nullptr != NameText);
	
	NameText->SetText(FText::FromString(InName));
}

void UMCOHpWidget::SetMaxHp(float NewMaxHp)
{
	MaxHp = NewMaxHp;
	
	ISTRUE(nullptr != HpProgressBar);
	HpProgressBar->SetPercent(CurrentHp / MaxHp);
}

void UMCOHpWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;
	
	ISTRUE(nullptr != HpProgressBar);
	HpProgressBar->SetPercent(CurrentHp / MaxHp);
}
