#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MCOPlayerSetting.generated.h"

UCLASS(Config=MonsterCO)
class MONSTERCO_API UMCOPlayerSetting : public UObject
{
	GENERATED_BODY()

public:
	float GetMasterVolume();
	
protected:
	UPROPERTY(Config)
	float MasterVolume;
};
