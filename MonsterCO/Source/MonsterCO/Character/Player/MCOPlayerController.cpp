#include "MCOPlayerController.h"
#include "MCOPlayerCharacter.h"
#include "Character/MCOPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MainWidgets/MCOMainWidget.h"
#include "UI/MainWidgets/MCOMainWidgetData.h"


AMCOPlayerController::AMCOPlayerController()
{
// 	GETCLASS(HUDWidgetClass, UMCOHUDWidget, TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
// 	GETCLASS(OptionWidgetClass, UMCOOptionWidget, TEXT("/Game/UI/WBP_Option.WBP_Option_C"));

	GETASSET(WidgetData, UMCOMainWidgetData, TEXT("/Game/Data/UI/DA_MainWidgetData.DA_MainWidgetData"));
}

void AMCOPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// move to pawn extension component
	// const AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	// if (nullptr != MCOPlayerState)
	// {
	// 	MCOPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, InPawn);
	// }
}

void AMCOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create HUD
	CreateWidgetAndAddToViewport(WidgetData->WidgetClasses[EMCOWidgetState::INGAME], EMCOWidgetState::INGAME);

	// OnGameStateChanged
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
}

void AMCOPlayerController::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::TITLE)
	{
		ChangeWidgetByState(EMCOWidgetState::TITLE);
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		ChangeWidgetByState(EMCOWidgetState::INGAME);
	}
	else if (InState == EMCOGameState::NEXT)
	{
		ChangeWidgetByState(EMCOWidgetState::NEXT);
	}
	else if (InState == EMCOGameState::RESULT_WIN || InState == EMCOGameState::RESULT_LOSE)
	{
		ChangeWidgetByState(EMCOWidgetState::RESULT);
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

void AMCOPlayerController::CreateWidgetAndAddToViewport(TSubclassOf<UMCOMainWidget>& InClass, EMCOWidgetState InState)
{
	UMCOMainWidget* Widget = CreateWidget<UMCOMainWidget>(GetWorld(), InClass);
	Widget->OnExitGameDelegate.BindUObject(this, &ThisClass::ExitGame);
	Widget->OnWidgetClosedByUserDelegate.BindUObject(this, &ThisClass::OnWidgetClosedByUser);
	Widget->AddToViewport();

	Widgets.Emplace(InState, Widget);
}

void AMCOPlayerController::ChangeInputMode(bool bGameMode)
{
	if (true == bGameMode)
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

void AMCOPlayerController::OnOptionKeyPressed()
{
	ChangeWidgetByState(EMCOWidgetState::OPTION);
}

void AMCOPlayerController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		this,
		EQuitPreference::Quit,
		true
	);
}

void AMCOPlayerController::ChangeWidgetByState(EMCOWidgetState InState)
{
	ISTRUE(true == WidgetData->WidgetClasses.Contains(InState));

	ChangeInputMode(InState == EMCOWidgetState::INGAME || InState == EMCOWidgetState::NEXT);
	
	if (true == Widgets.Contains(CurrentWidgetState))
	{
		// If next widget is OPTION -> have to go back anyway. So don't destroy.
		// If current widget is HUD -> don't destroy HUD
		if (InState != EMCOWidgetState::OPTION && CurrentWidgetState != EMCOWidgetState::INGAME)
		{
			Widgets[CurrentWidgetState]->RemoveFromParent();
		}
		else
		{
			Widgets[CurrentWidgetState]->OnClosed();
		}
	}

	// If current widget is OPTION -> you just closed prev widget(not destroyed) when opening OPTION. So no need to create.
	// If next widget is HUD -> HUD is always alive. So no need to create, except for init. 
	if (false == Widgets.Contains(InState) ||
		(CurrentWidgetState != EMCOWidgetState::OPTION && InState != EMCOWidgetState::INGAME))
	{
		CreateWidgetAndAddToViewport(WidgetData->WidgetClasses[InState], InState);
	}
	Widgets[InState]->OnShow();

	PreviousWidgetState = CurrentWidgetState;
	CurrentWidgetState = InState;
}

void AMCOPlayerController::OnWidgetClosedByUser()
{
	if (CurrentWidgetState == EMCOWidgetState::OPTION)
	{
		ChangeWidgetByState(PreviousWidgetState);
	}
}
