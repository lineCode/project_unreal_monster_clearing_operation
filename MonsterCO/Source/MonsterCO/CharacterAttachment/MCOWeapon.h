#pragma once

#include "MonsterCO.h"
#include "MCOAttachment.h"
#include "MCOWeapon.generated.h"

class UStaticMeshComponent;


UCLASS()
class MONSTERCO_API AMCOWeapon : public AMCOAttachment
{
	GENERATED_BODY()
	
public:	
	AMCOWeapon();

protected:
	virtual void BeginPlay() override;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MCO, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	
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
