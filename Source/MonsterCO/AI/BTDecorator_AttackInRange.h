#pragma once

#include "MonsterCO.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

UCLASS()
class MONSTERCO_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_AttackInRange();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};