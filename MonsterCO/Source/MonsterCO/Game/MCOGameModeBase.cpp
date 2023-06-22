#include "MCOGameModeBase.h"
#include "Character/Player/MCOPlayerCharacter.h"
#include "GameFramework/PlayerStart.h"


AMCOGameModeBase::AMCOGameModeBase()
{
	// DefaultPawnClass      = AMCOPlayerCharacter::StaticClass();
	// PlayerControllerClass = AMCOPlayerController::StaticClass();
	// PlayerStateClass      = AMCOPlayerState::StaticClass();

	CurrentStage = 1;
}

void AMCOGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	OnChangeGameState(EMCOGameState::LOBBY);
}

void AMCOGameModeBase::OnChangeGameState(const EMCOGameState& InState)
{
	MCOLOG_C(MCOAbility, TEXT("GameState Changed to : [%s]"), *FHelper::GetEnumDisplayName(TEXT("EMCOGameState"), (int64)InState));
	
	CurrentGameState = InState;
	
	if (InState == EMCOGameState::MOVE_TO_NEXT_STAGE)
	{
		CurrentStage++;

		ISTRUE(true == OnGameStateChangedDelegate.IsBound());
		OnGameStateChangedDelegate.Broadcast(InState);
		
		OnChangeGameState(EMCOGameState::FIGHT);

		return;
	}
	else if (InState == EMCOGameState::RESTART_STAGE_AFTER_LOSE)
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

		ISTRUE(true == OnGameStateChangedDelegate.IsBound());
		OnGameStateChangedDelegate.Broadcast(InState);
		
		OnChangeGameState(EMCOGameState::FIGHT);

		return;
	}
	else if (InState == EMCOGameState::MONSTER_DIED)
	{
		if (MAX_STAGE <= CurrentStage)
		{
			OnChangeGameState(EMCOGameState::RESULT_WIN);
		}
		else
		{
			OnChangeGameState(EMCOGameState::REWARD);
		}		
		return;
	}
	else if (InState == EMCOGameState::REWARD)
	{
		
	}

	ISTRUE(true == OnGameStateChangedDelegate.IsBound());
	OnGameStateChangedDelegate.Broadcast(InState);
}
