#include "MCOPlayerController.h"
#include "MCOPlayerCharacter.h"
#include "Character/MCOPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MCOHUDWidget.h"


AMCOPlayerController::AMCOPlayerController()
{
	GETCLASS(HUDWidgetClass, UMCOHUDWidget, TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
}

void AMCOPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	if (nullptr != MCOPlayerState)
	{
		MCOPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, InPawn);
	}
}

void AMCOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UMCOHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();

	// OnGameStateChanged
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
}

void AMCOPlayerController::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::LOBBY)
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	else if (InState == EMCOGameState::REWARD)
	{
	}
	else if (InState == EMCOGameState::RESULT_WIN || InState == EMCOGameState::RESULT_LOSE)
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
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
