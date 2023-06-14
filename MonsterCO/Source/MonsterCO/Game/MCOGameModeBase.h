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

// --- Interface
public:
	virtual const int32& GetStage() const override { return CurrentStage; }
	virtual const EMCOGameState& GetGameState() const override { return CurrentGameState; }
	virtual FOnGameStateChangedDelegate& GetOnGameStateChangedDelegate() override { return OnGameStateChangedDelegate; }
	virtual void OnChangeGameState(const EMCOGameState& InState) override;
	
protected:
	FOnGameStateChangedDelegate OnGameStateChangedDelegate;

	UPROPERTY()
	EMCOGameState CurrentGameState;
	
	UPROPERTY()
	int32 CurrentStage;
};
