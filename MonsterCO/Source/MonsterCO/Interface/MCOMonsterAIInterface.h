#pragma once

#include "CoreMinimal.h"
#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/Interface.h"
#include "MCOMonsterAIInterface.generated.h"

DECLARE_DELEGATE_OneParam(FAICharacterAITaskFinishedDelegate, EBTNodeResult::Type /*Result*/);


UINTERFACE(MinimalAPI)
class UMCOMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOMonsterAIInterface
{
	GENERATED_BODY()

public:
	FAICharacterAITaskFinishedDelegate OnAttackFinished;
	FAICharacterAITaskFinishedDelegate OnTurnFinished;

public:
	virtual UObject* GetTarget() = 0;
	
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRangeMin(const FGameplayTag& InTag) = 0;
	virtual float GetAIAttackRangeMax(const FGameplayTag& InTag) = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual FVector GetAITurnVector() = 0;

	virtual bool IsTurning() = 0;
	virtual void SetTurnVector(const bool InIsTurning, const FVector& InTurnVector = FVector()) = 0;
	virtual void SetAIAttackDelegate(const FAICharacterAITaskFinishedDelegate& InOnAttackFinished) = 0;

	virtual void Attack(const FGameplayTag& InTag) = 0;
};
