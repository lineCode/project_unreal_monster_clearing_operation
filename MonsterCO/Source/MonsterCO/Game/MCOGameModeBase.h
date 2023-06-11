#pragma once

#include "MonsterCO.h"
#include "GameFramework/GameModeBase.h"
#include "MCOGameModeBase.generated.h"

UCLASS()
class MONSTERCO_API AMCOGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMCOGameModeBase();

public:
	virtual void StartPlay() override;
	
	FORCEINLINE int32 GetPhase() const { return CurrentPhase; }

// --- Monster
protected:
	UPROPERTY()
	int32 CurrentPhase;
};
