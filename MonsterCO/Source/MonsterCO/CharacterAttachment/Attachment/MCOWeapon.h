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
	void SetEquipUnequipInstantly(bool bEquip);
	void SwitchEquipUnequip(bool bEquip);
	void BeginAnimation_Equip();
	void EndAnimation_Equip();

protected:
	UPROPERTY()
	uint8 bToEquip:1;
	
//--- Socket
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnequipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipSocketName;
};
