#include "UI/MCOStaminaWidget.h"
#include "Components/Image.h"
#include "Interface/MCOHUDInterface.h"


void UMCOStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	StaminaRadialProgressBar = Cast<UImage>(GetWidgetFromName(TEXT("RadialProgressBar")));
	ensure(StaminaRadialProgressBar);
	StaminaRadialProgressBar->SetVisibility(ESlateVisibility::Visible);

	bIsTimerTicking = false;
	
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
		bIsTimerTicking = true;
		
		GetWorld()->GetTimerManager().SetTimer(
			StaminaTimerHandle,
			this,
			&ThisClass::OnStaminaFull,
			1.5f,
			false
		);
	}
	else
	{
		if (true == bIsTimerTicking)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		}
		else
		{
			SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMCOStaminaWidget::OnStaminaFull()
{
	bIsTimerTicking = false;
	SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	StaminaTimerHandle.Invalidate();
}
