#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCOPlayerModeData.generated.h"

class AMCOWeapon;


UCLASS()
class MONSTERCO_API UMCOPlayerModeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TMap<EMCOModeType, TSubclassOf<AMCOWeapon>> WeaponClasses;
};
