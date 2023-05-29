#pragma once

#include "MonsterCO.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

UCLASS()
class MONSTERCO_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
		
public:
	UBTTask_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	FRotator TargetRot;
	FRotator PrevTargetRot;
};
