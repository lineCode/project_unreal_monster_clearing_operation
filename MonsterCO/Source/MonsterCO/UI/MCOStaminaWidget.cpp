#include "UI/MCOStaminaWidget.h"
#include "Components/Image.h"
#include "Interface/MCOHUDInterface.h"


void UMCOStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	StaminaRadialProgressBar = Cast<UImage>(GetWidgetFromName(TEXT("RadialProgressBar")));
	ensure(StaminaRadialProgressBar);
	StaminaRadialProgressBar->SetVisibility(ESlateVisibility::Visible);

	IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(OwningActor);
	if (nullptr != HUDInterface)
	{
		HUDInterface->SetupStaminaWidget(this);
	}
}

void UMCOStaminaWidget::SetPercent(const float& InPercent)
{
	SetImagePercent(InPercent);

	if (InPercent >= 1.0f)
	{
		StaminaTimerHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(
			StaminaTimerHandle,
			this,
			&ThisClass::OnStaminaFull,
			3.0f,
			false
		);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UMCOStaminaWidget::OnStaminaFull()
{	
	SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	StaminaTimerHandle.Invalidate();
}
