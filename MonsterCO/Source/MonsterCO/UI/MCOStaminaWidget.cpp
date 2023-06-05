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

void UMCOStaminaWidget::SetWidgetValues(const float& InCurrentValue, const float& InMaxStamina)
{
	MCOLOG(TEXT("Set Widget Stamina : %f / %f"), InCurrentValue, InMaxStamina);
	
	CurrentValue = InCurrentValue;
	MaxValue = InMaxStamina;

	SetImagePercent(CurrentValue / MaxValue);
}

void UMCOStaminaWidget::StartWidget(const float& InAdditiveValue)
{
	StartValue = CurrentValue;
	AdditiveValue += InAdditiveValue;

	if (GetWorld()->GetTimerManager().GetTimerRemaining(StaminaTimerHandle) <= 0.0f)
	{
		StartUpdatingStaminaWidget();
	}
}

void UMCOStaminaWidget::StartUpdatingStaminaWidget()
{
	StaminaRadialProgressBar->SetVisibility(ESlateVisibility::Visible);

	ResetTimer();
	
	GetWorld()->GetTimerManager().SetTimer(
		StaminaTimerHandle,
		this,
		&ThisClass::UpdateStaminaProgressBar,
		WIDGET_RATE,
		true,
		0.0f
	);
}

void UMCOStaminaWidget::UpdateStaminaProgressBar()
{
	ensure(MaxValue != 0.0f);

	CurrentValue += AdditiveValue * WIDGET_RATE;
	SetImagePercent(CurrentValue / MaxValue);

	if (CurrentValue >= MaxValue)
	{
		OnStaminaFull();
	}
	if (CurrentValue <= 0.0f)
	{
		OnStaminaEnd();
	}
	if (true == FMath::IsNearlyEqual(CurrentValue, StartValue + AdditiveValue))
	{
		OnStaminaEnd();
	}
}

void UMCOStaminaWidget::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
}

void UMCOStaminaWidget::OnStaminaEnd()
{
	AdditiveValue = 0;
	ResetTimer();
}

void UMCOStaminaWidget::OnStaminaFull()
{
	CurrentValue = MaxValue;
	AdditiveValue = 0;
	
	// StaminaRadialProgressBar->SetVisibility(ESlateVisibility::Hidden);
	ResetTimer();
}
