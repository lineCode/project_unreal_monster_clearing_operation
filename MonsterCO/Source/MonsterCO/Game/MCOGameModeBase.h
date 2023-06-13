#pragma once

#include "MonsterCO.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/MCOGameModeInterface.h"
#include "MCOGameModeBase.generated.h"


class AMCOPlayerCharacter;

UCLASS()
class MONSTERCO_API AMCOGameModeBase : public AGameModeBase, public IMCOGameModeInterface
{
	GENERATED_BODY()
	
public:
	AMCOGameModeBase();

public:
	virtual void StartPlay() override;
	
	virtual const int32& GetPhase() const override { return CurrentStage; }
	virtual const EMCOGameState& GetGameState() const override { return CurrentGameState; }
	virtual FOnGameStateChangedDelegate& GetOnGameStateChangedDelegate() override { return OnGameStateChangedDelegate; }
	virtual FOnGameResultDelegate& GetOnGameResultDelegate() override { return OnGameResultDelegate; }
	virtual FOnRestartStageDelegate& GetOnRestartStageDelegate() override { return OnRestartStageDelegate; }

	virtual void OnRestartStage() override;
	virtual void OnChangeGameState(const EMCOGameState& InState) override;
	virtual void OnGameResult(const bool bWin) override;
	
// --- State
protected:
	FOnRestartStageDelegate OnRestartStageDelegate;
	FOnGameStateChangedDelegate OnGameStateChangedDelegate;
	FOnGameResultDelegate OnGameResultDelegate;

	UPROPERTY()
	EMCOGameState CurrentGameState;
	
	UPROPERTY()
	int32 CurrentStage;
};
