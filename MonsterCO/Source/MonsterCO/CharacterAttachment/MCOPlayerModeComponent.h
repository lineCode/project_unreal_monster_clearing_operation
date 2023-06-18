#pragma once

#include "MonsterCO.h"
#include "MCOModeComponent.h"
#include "MCOPlayerModeComponent.generated.h"

class AMCOWeapon;
class UMCOPlayerModeData; 

UCLASS()
class MONSTERCO_API UMCOPlayerModeComponent : public UMCOModeComponent
{
	GENERATED_BODY()

public:	
	UMCOPlayerModeComponent();

// --- Spawn
public:
	virtual void SpawnAttachment(ACharacter* InOwner) override;

protected:
	UPROPERTY()
	TObjectPtr<UMCOPlayerModeData> ModeData;

// --- Equip
public:
	virtual void SetEquipUnequipInstantly(const bool bEquip) override;
	virtual void SwitchEquipUnequip() override;

protected:
	UPROPERTY()
	uint8 bIsToEquip:1;

};
