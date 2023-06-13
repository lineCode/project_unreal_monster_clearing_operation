#include "MCOGameModeBase.h"
#include "Character/Player/MCOPlayerCharacter.h"
#include "GameFramework/PlayerStart.h"


AMCOGameModeBase::AMCOGameModeBase()
{
	// DefaultPawnClass      = AMCOPlayerCharacter::StaticClass();
	// PlayerControllerClass = AMCOPlayerController::StaticClass();
	// PlayerStateClass      = AMCOPlayerState::StaticClass();

	CurrentStage = 0;
	CurrentGameState = EMCOGameState::LOBBY;
}

void AMCOGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	OnChangeGameState(EMCOGameState::LOBBY);
}

void AMCOGameModeBase::OnRestartStage()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (0 < PlayerStarts.Num() && nullptr != DefaultPawnClass)
	{
		AMCOPlayerCharacter* Player = GetWorld()->SpawnActor<AMCOPlayerCharacter>(
			DefaultPawnClass,
			PlayerStarts[0]->GetActorLocation(),
			PlayerStarts[0]->GetActorRotation()
		);
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (nullptr != PlayerController)
		{
			PlayerController->Possess(Player);
		}
	}

	OnChangeGameState(EMCOGameState::FIGHT);

	ISTRUE(true == OnRestartStageDelegate.IsBound());
	OnRestartStageDelegate.Broadcast();
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
