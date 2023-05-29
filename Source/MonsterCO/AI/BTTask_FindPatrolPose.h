#pragma once

#include "MonsterCO.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPose.generated.h"

UCLASS()
class MONSTERCO_API UBTTask_FindPatrolPose : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindPatrolPose();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
