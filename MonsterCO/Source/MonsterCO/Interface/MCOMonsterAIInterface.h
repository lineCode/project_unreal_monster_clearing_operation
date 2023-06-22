#pragma once

#include "CoreMinimal.h"
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

// --- AI/State	
public:
	virtual FVector GetAITurnVector() const = 0;
	virtual bool IsTurning() const = 0;
	virtual void SetTurnVector(const bool InIsTurning, const FVector& InTurnVector = FVector()) = 0;
	virtual void SetActionDelegate(const FMCOAICharacterTaskFinishedDelegate& InOnActionFinished) = 0;
	virtual void OnActionFinished(const EBTNodeResult::Type& InResult) const = 0;

// --- AI/Action
public:
	virtual void Attack(const FGameplayTag& InTag) const = 0;
};
