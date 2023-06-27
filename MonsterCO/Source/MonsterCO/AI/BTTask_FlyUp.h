#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyUp.generated.h"



UCLASS()
class MONSTERCO_API UBTTask_FlyUp : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FlyUp();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	float FlyDistance;

	UPROPERTY(EditAnywhere)
	float FlyForce;
		
	UPROPERTY()
	FVector FlyGoal;
};
