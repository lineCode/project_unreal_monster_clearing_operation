#pragma once

#include "MonsterCO.h"
#include "UObject/NoExportTypes.h"
#include "MCOGameSingleton.generated.h"

UCLASS()
class MONSTERCO_API UMCOGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMCOGameSingleton();
	static UMCOGameSingleton& Get();

};
