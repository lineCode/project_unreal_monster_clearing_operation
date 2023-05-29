#pragma once

#include "MonsterCO.h"
#include "Character/MCOAttachment.h"
#include "MCOWeapon.generated.h"

UCLASS()
class MONSTERCO_API AMCOWeapon : public AMCOAttachment
{
	GENERATED_BODY()
	
public:	
	AMCOWeapon();

protected:
	virtual void BeginPlay() override;

//--- Equip
public:
	void SwitchEquipUnequip(bool bInToEquip);
	void BeginAnimation_Equip();
	void EndAnimation_Equip();

protected:
	UPROPERTY()
	uint8 bIsToEquip:1;

//--- Socket
protected:
	UPROPERTY()
	FName HolsterSocketName;

	UPROPERTY()
	FName HandSocketName;
};
