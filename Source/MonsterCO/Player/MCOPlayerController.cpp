#include "MCOPlayerController.h"
#include "MCOPlayerCharacter.h"
#include "MCOPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Ability_Character/MCOAbilitySystemComponent.h"
#include "UI/MCOHUDWidget.h"


AMCOPlayerController::AMCOPlayerController()
{
	GETCLASS(HUDWidgetClass, UMCOHUDWidget, TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
}

void AMCOPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	if (nullptr != MCOPlayerState)
	{
		MCOPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, InPawn);
	}
}

void AMCOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UMCOHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}

UMCOAbilitySystemComponent* AMCOPlayerController::GetMCOAbilitySystemComponent() const
{
	const AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	return MCOPlayerState->GetMCOAbilitySystemComponent();
}

void AMCOPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	UMCOAbilitySystemComponent* MCOASC = GetMCOAbilitySystemComponent();
	if (nullptr != MCOASC)
	{
		MCOASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
