#pragma once

#include "MonsterCO.h"
#include "Components/ActorComponent.h"
#include "MCOPlayerModeComponent.generated.h"

class AMCOWeapon;
class UMCOPlayerModeData; 

UCLASS()
class MONSTERCO_API UMCOPlayerModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMCOPlayerModeComponent();

public:
	FORCEINLINE AMCOWeapon* GetWeapon() { return CurrentWeaponActor; }
	FORCEINLINE const EMCOModeType GetModeType() const { return CurrentModeType; }

	FORCEINLINE bool IsEquipped() const { return bIsEquipped; }
	
	void SpawnWeapon(ACharacter* InOwner);
	void SetMode(const EMCOModeType InModeType);
	void SwitchEquipUnequip();

protected:
	UPROPERTY()
	uint8 bIsEquipped:1;

	UPROPERTY()
	TObjectPtr<UMCOPlayerModeData> ModeData;
	
	UPROPERTY()
	TObjectPtr<AMCOWeapon> CurrentWeaponActor;
	
	UPROPERTY()
	EMCOModeType CurrentModeType;
};
