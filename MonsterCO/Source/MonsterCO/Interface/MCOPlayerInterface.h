#pragma once

#include "MonsterCO.h"
#include "UObject/Interface.h"
#include "MCOPlayerInterface.generated.h"



UINTERFACE(MinimalAPI)
class UMCOPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOPlayerInterface
{
	GENERATED_BODY()

// --- Action
public:
	virtual void SetSpeed(const EMCOCharacterSpeed& CharacterSpeed) const = 0;
	
	virtual FVector GetInputWorldDirection() const = 0;
	virtual bool IsInputForward() const = 0;
	
// --- Weapon
public:
	virtual EMCOPlayerMode GetModeType() const = 0;
	virtual bool IsEquipped() = 0;
	virtual void EquipInstantly() = 0;
	virtual void SwitchEquipUnequip() = 0;
	virtual void BeginAnimation_Equip() = 0;
	virtual void EndAnimation_Equip() = 0;

};
