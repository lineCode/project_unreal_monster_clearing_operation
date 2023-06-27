#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/Interface.h"
#include "MCOMonsterAIInterface.generated.h"

DECLARE_DELEGATE_OneParam(FMCOAICharacterTaskFinishedDelegate, const EBTNodeResult::Type& /*InResult*/);


UINTERFACE(MinimalAPI)
class UMCOMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOMonsterAIInterface
{
	GENERATED_BODY()

public:
	virtual UObject* GetTarget() = 0;

// --- AI/Data
public:
	virtual float GetAIPatrolRadius() const = 0;
	virtual float GetAIDetectRange() const = 0;
	virtual float GetAIAttackRangeMin(const FGameplayTag& InTag) const = 0;
	virtual float GetAIAttackRangeMax(const FGameplayTag& InTag) const = 0;
	virtual float GetAITurnSpeed() const = 0;
	virtual float GetAIFlySpeed() const = 0;
	virtual float GetHalfHeight() const = 0;

// --- AI/State	
public:
	virtual void SetMovementMode(EMovementMode InMode) = 0;
	virtual void SetGravity(float InGravity) = 0;
	virtual void SetFlyMode(EMCOMonsterFlyMode InFlyMode) = 0;
	virtual EMCOMonsterFlyMode GetFlyMode() = 0;
	virtual void AddForce(FVector InForce) = 0;
	virtual void SetVelocity(FVector InVelocity) = 0;
	virtual FVector GetVelocity() = 0;
	virtual bool IsTurning() const = 0;
	virtual FVector GetTurnVector() const = 0;
	virtual void SetTurnVector(const FVector& InTurnVector = FVector()) = 0;
	
	virtual void SetActionDelegate(const FMCOAICharacterTaskFinishedDelegate& InOnActionFinished) = 0;
	virtual void OnActionFinished(const EBTNodeResult::Type& InResult) const = 0;
	virtual void SetDamagedInBlackBoard(bool IsDamaged) const = 0;

// --- AI/Action
public:
	virtual bool Attack(const FGameplayTag& InTag) const = 0;
};
