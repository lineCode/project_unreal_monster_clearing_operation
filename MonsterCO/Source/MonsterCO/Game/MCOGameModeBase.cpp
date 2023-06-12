#include "MCOGameModeBase.h"


AMCOGameModeBase::AMCOGameModeBase()
{
	// DefaultPawnClass      = AMCOPlayerCharacter::StaticClass();
	// PlayerControllerClass = AMCOPlayerController::StaticClass();
	// PlayerStateClass      = AMCOPlayerState::StaticClass();

	CurrentPhase = 0;
	CurrentGameState = EMCOGameState::READY;
}

void AMCOGameModeBase::StartPlay()
{
	Super::StartPlay();
	
}
