#pragma once

#include "MonsterCO.h"
#include "MCOGameSingleton.generated.h"

// Not used

UCLASS()
class MONSTERCO_API UMCOGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMCOGameSingleton();
	static UMCOGameSingleton& Get();
	
};
