#include "MCOGameModeBase.h"
#include "Character/Player/MCOPlayerController.h"


AMCOGameModeBase::AMCOGameModeBase()
{
	// DefaultPawnClass      = AMCOPlayerCharacter::StaticClass();
	// PlayerControllerClass = AMCOPlayerController::StaticClass();
	// PlayerStateClass      = AMCOPlayerState::StaticClass();

	CurrentPhase = 0;
	CurrentGameState = EMCOGameState::LOBBY;
}

void AMCOGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	OnChangeGameState(EMCOGameState::LOBBY);
}

void AMCOGameModeBase::OnChangeGameState(const EMCOGameState& InState)
{
	CurrentGameState = InState;
	
	ISTRUE(true == OnGameStateChangedDelegate.IsBound());
	OnGameStateChangedDelegate.Broadcast(InState);
}

void AMCOGameModeBase::OnGameResult(const bool bWin)
{
	if (true == bWin)
	{
		//CurrentPhase++;
	}
	ISTRUE(true == OnGameResultDelegate.IsBound());
	OnGameResultDelegate.Broadcast(bWin);
}
