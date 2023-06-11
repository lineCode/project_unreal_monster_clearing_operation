#include "MCOGameModeBase.h"
#include "Sound/MCOGameSingleton.h"


AMCOGameModeBase::AMCOGameModeBase()
{
	// DefaultPawnClass      = AMCOPlayerCharacter::StaticClass();
	// PlayerControllerClass = AMCOPlayerController::StaticClass();
	// PlayerStateClass      = AMCOPlayerState::StaticClass();

	CurrentPhase = 0;
}

void AMCOGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	UMCOGameSingleton::Get().PlaySoundByGameState(GetWorld(), EMCOGameState::READY);
}
