#pragma once

#include "MonsterCO.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/MCOGameModeInterface.h"
#include "MCOGameModeBase.generated.h"



UCLASS()
class MONSTERCO_API AMCOGameModeBase : public AGameModeBase, public IMCOGameModeInterface
{
	GENERATED_BODY()
	
public:
	AMCOGameModeBase();

public:
	virtual void StartPlay() override;
	
	virtual const int32& GetPhase() const override { return CurrentPhase; }
	virtual const EMCOGameState& GetGameState() const override { return CurrentGameState; }
	virtual FOnGameStateChanged& GetOnGameStateChangedDelegate() override { return OnGameStateChangedDelegate; }
	virtual FOnGameResult& GetOnGameResultDelegate() override { return OnGameResultDelegate; }

	virtual void OnChangeGameState(const EMCOGameState& InState) override;
	virtual void OnGameResult(const bool bWin) override;
	
// --- State
protected:
	FOnGameStateChanged OnGameStateChangedDelegate;
	FOnGameResult OnGameResultDelegate;

	UPROPERTY()
	EMCOGameState CurrentGameState;
	
	UPROPERTY()
	int32 CurrentPhase;
};
