#pragma once

#include "MonsterCO.h"
#include "UObject/Interface.h"
#include "MCOGameModeInterface.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedDelegate, const EMCOGameState&, InState);



UINTERFACE(MinimalAPI)
class UMCOGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOGameModeInterface
{
	GENERATED_BODY()

public:
	virtual const int32& GetStage() const = 0;
	virtual const EMCOGameState& GetGameState() const = 0;

	UFUNCTION()
	virtual FOnGameStateChangedDelegate& GetOnGameStateChangedDelegate() = 0;
	
	virtual void OnChangeGameState(const EMCOGameState& InState) = 0;
};
