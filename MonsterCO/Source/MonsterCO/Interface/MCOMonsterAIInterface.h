#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/Interface.h"
#include "MCOMonsterAIInterface.generated.h"

DECLARE_DELEGATE_OneParam(FMCOActionFinishedDelegate, EBTNodeResult::Type /*InResult*/);
DECLARE_DELEGATE(FMCOActivateActionDelegate);


UINTERFACE(MinimalAPI)
class UMCOMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOMonsterAIInterface
{
	GENERATED_BODY()

// --- Ability
public:
	virtual bool TryActivateAbilityByTag(const FGameplayTag& InTag) const = 0;
	virtual void CancelAbilityByTag(const FGameplayTag& InTag) const = 0;
	
// --- AI/Data
public:
	virtual UObject* GetTarget() = 0;
	virtual float GetAIPatrolRadius() const = 0;
	virtual float GetAIDetectRange() const = 0;
	virtual float GetAIAttackRangeMin(const FGameplayTag& InTag) const = 0;
	virtual float GetAIAttackRangeMax(const FGameplayTag& InTag) const = 0;
	virtual float GetAITurnSpeed() const = 0;
	virtual float GetAIFlySpeed() const = 0;
	virtual float GetHalfHeight() const = 0;

// --- AI/Fly	
public:
	virtual void SetMovementMode(EMovementMode InMode) = 0;
	virtual void SetGravity(float InGravity) = 0;
	virtual void SetFlyMode(EMCOMonsterFlyMode InFlyMode) = 0;
	virtual EMCOMonsterFlyMode GetFlyMode() = 0;
	virtual void AddForce(FVector InForce) = 0;
	virtual void SetVelocity(FVector InVelocity) = 0;
	virtual FVector GetVelocity() = 0;

// --- AI/Turn
public:
	virtual bool IsTurning() const = 0;
	virtual FVector GetTurnVector() const = 0;
	virtual void SetTurnVector(FVector InTurnVector) = 0;

	
// --- AI/Task
public:
	virtual void SetActivateActionDelegate(const FMCOActivateActionDelegate& InOnActivateAction) = 0;
	virtual void OnActivateAction() = 0;
	virtual void SetActionFinishedDelegate(const FMCOActionFinishedDelegate& InOnActionFinished) = 0;
	virtual void OnActionFinished(EBTNodeResult::Type InResult) = 0;
	virtual void SetDamagedInBlackBoard(bool IsDamaged) const = 0;

};
