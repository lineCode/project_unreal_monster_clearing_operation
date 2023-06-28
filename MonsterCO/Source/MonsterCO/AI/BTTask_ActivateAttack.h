#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAttack.generated.h"




UCLASS()
class MONSTERCO_API UBTTask_ActivateAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
