#pragma once

#include "MonsterCO.h"
#include "UObject/Interface.h"
#include "MCOCharacterInterface.generated.h"

class UMCOHpWidget;
class UMCOAttributeWidget;

USTRUCT()
struct FMCODamagedData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float AttackedDegree = 0.0f;
};


DECLARE_DELEGATE(FOnDamagedDelegate);


UINTERFACE(MinimalAPI)
class UMCOCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOCharacterInterface
{
	GENERATED_BODY()

public:	
// --- Action
	virtual bool CanActionWithStamina(const float& InStaminaUsage) const = 0;	
	virtual bool CanChargeStamina() const = 0;
	virtual void OffAllCollision() = 0;
	virtual bool CanAttack() const = 0;
	virtual void StopCharacter(bool InToStop) = 0;
	virtual float GetCapsuleRadius() const = 0;

// --- Damaged
public:
	FOnDamagedDelegate OnDamagedBegin;
	FOnDamagedDelegate OnDamagedEnd;

	virtual const FMCODamagedData GetDamagedData() = 0;
	virtual void SetDamagedData(const FMCODamagedData& InDegree) = 0;

// --- Die
public:
	virtual void Die() = 0;
	virtual void FinishDying() = 0;

// --- Widget
public:
	virtual void InitializeWidget(UMCOHpWidget* InHpWidget, UMCOAttributeWidget* InAttributeWidget) = 0;
};
