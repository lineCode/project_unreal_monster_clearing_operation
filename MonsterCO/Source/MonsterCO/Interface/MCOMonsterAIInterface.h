#pragma once

#include "CoreMinimal.h"
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
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIMeleeRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual FVector GetAITurnVector() = 0;
	
	virtual void SetTurnVector(const FVector& InTurnVector) = 0;
	virtual void SetAIAttackDelegate(const FAICharacterAITaskFinishedDelegate& InOnAttackFinished) = 0;

	virtual bool CanAttack() = 0;
	virtual void AttackByAI() = 0;
};
