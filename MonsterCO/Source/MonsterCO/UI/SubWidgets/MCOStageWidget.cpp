#include "UI/SubWidgets/MCOStageWidget.h"
#include "Components/TextBlock.h"


void UMCOStageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CurrentStageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentStageText")));
	MaxStageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxStageText")));

	ensure(nullptr != CurrentStageText);
	ensure(nullptr != MaxStageText);

	MaxStageText->SetText(
		FText::FromString(FString::FromInt(MAX_STAGE))
	);
}

void UMCOStageWidget::OnStageChanged(const int32& InCurrent) const
{
	CurrentStageText->SetText(
		FText::FromString(FString::FromInt(InCurrent))
	);
}
