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
	virtual bool CanJumpAction() const = 0;
	virtual bool CanEquipAction() const = 0;
	virtual bool CanDodgeAction() const = 0;
	virtual bool CanDashAction() const = 0;
	virtual void SetSpeed(const EMCOCharacterSpeed& CharacterSpeed) const = 0;
	
	virtual FVector GetInputWorldDirection() const = 0;
	virtual bool IsInputForward() const = 0;
	
// --- Weapon
public:
	virtual EMCOModeType GetModeType() const = 0;
	virtual bool IsEquipped() = 0;
	virtual void SetEquippedWithoutAnimation() = 0;
	virtual void SwitchEquipUnequip() = 0;
	virtual void BeginAnimation_Equip() = 0;
	virtual void EndAnimation_Equip() = 0;

};
