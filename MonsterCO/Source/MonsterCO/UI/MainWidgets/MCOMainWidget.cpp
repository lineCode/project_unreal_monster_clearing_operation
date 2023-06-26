#include "UI/MainWidgets/MCOMainWidget.h"

void UMCOMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UMCOMainWidget::OnShow()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UMCOMainWidget::OnClosed()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UMCOMainWidget::ExitGame()
{
	OnExitGameDelegate.ExecuteIfBound();
}
