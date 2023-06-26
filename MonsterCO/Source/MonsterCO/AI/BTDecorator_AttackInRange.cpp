#include "BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MCOAIKeys.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("Is in attack range");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult =  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr ==  ControllingPawn)
	{
		return false;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	const float AttackRangeMin = AIPawn->GetAIAttackRangeMin(AttackTag);
	const float AttackRangeMax = AIPawn->GetAIAttackRangeMax(AttackTag);
	const float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	bResult = (AttackRangeMin <= DistanceToTarget) && (DistanceToTarget <= AttackRangeMax);

	//MCOLOG_C(MCOMonsterAI, TEXT("%f <= %f <= %f"), AttackRangeMin, DistanceToTarget, AttackRangeMax);
	
#if ENABLE_DRAW_DEBUG
	//DrawDebugSphere(GetWorld(), ControllingPawn->GetActorLocation(), AttackRangeMin, 16,  (bResult == true) ? FColor::Blue : FColor::Yellow, false, (true == bResult) ? 1.0f : 0.3f);
	//DrawDebugSphere(GetWorld(), ControllingPawn->GetActorLocation(), AttackRangeMax, 16,  (bResult == true) ? FColor::Blue : FColor::Yellow, false, (true == bResult) ? 1.0f : 0.3f);
#endif
	
	return bResult;
}
