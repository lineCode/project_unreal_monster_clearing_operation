#pragma once

#include "MonsterCO.h"
#include "UObject/Interface.h"
#include "MCOGameModeInterface.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, const EMCOGameState&, InState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameResult, const bool, bWin);



UINTERFACE(MinimalAPI)
class UMCOGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOGameModeInterface
{
	GENERATED_BODY()

public:
	virtual const int32& GetPhase() const = 0;
	virtual const EMCOGameState& GetGameState() const = 0;

	UFUNCTION()
	virtual FOnGameStateChanged& GetOnGameStateChangedDelegate() = 0;
	
	UFUNCTION()
	virtual FOnGameResult& GetOnGameResultDelegate() = 0;

	virtual void OnChangeGameState(const EMCOGameState& InState) = 0;
	virtual void OnGameResult(const bool bWin) = 0;
};
