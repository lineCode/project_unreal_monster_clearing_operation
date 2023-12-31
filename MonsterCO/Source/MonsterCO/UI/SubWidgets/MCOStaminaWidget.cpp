#include "MCOStaminaWidget.h"
#include "Components/Image.h"
#include "Interface/MCOHUDInterface.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


void UMCOStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	StaminaRadialProgressBar = Cast<UImage>(GetWidgetFromName(TEXT("RadialProgressBar")));
	ensure(nullptr != StaminaRadialProgressBar);

	bIsTimerTicking = false;
	
	IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(OwningActor);
	if (nullptr != HUDInterface)
	{
		HUDInterface->SetupStaminaWidget(this);
	}

	// OnGameStateChanged
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UMCOStaminaWidget::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::RESTART_STAGE_AFTER_LOSE)
	{
		IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(OwningActor);
		if (nullptr != HUDInterface)
		{
			HUDInterface->SetupStaminaWidget(this);
		}
	}
	// else if (InState == EMCOGameState::FIGHT)
	// {
	// 	SetVisibility(ESlateVisibility::Visible);
	// }
	else if (InState == EMCOGameState::RESULT_WIN || InState == EMCOGameState::RESULT_LOSE)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMCOStaminaWidget::SetPercent(const float& InPercent)
{	
	SetImagePercent(InPercent);

	if (InPercent >= 1.0f)
	{
		bIsTimerTicking = true;

		StaminaTimerHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(
			StaminaTimerHandle,
			this,
			&ThisClass::OnStaminaFull,
			2.0f,
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
}
