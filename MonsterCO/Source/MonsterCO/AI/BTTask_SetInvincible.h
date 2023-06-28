#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetInvincible.generated.h"

UCLASS()
class MONSTERCO_API UBTTask_SetInvincible : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetInvincible();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category = Attack)
	uint8 bSetInvincible : 1;
};
