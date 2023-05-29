#include "BTTask_Attack.h"
#include "AIController.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr ==  ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	if (false == AIPawn->CanAttack())
	{
		return EBTNodeResult::Failed;
	}
	
	FAICharacterAITaskFinishedDelegate OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&](EBTNodeResult::Type bInResult)
		{
			FinishLatentTask(OwnerComp, bInResult);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
