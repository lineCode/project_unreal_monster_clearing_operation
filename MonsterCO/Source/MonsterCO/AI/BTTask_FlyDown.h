#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyDown.generated.h"

UCLASS()
class MONSTERCO_API UBTTask_FlyDown : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FlyDown();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	float FlyForce;
		
	UPROPERTY()
	FVector FlyGoal;
};
