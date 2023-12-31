#include "AI/BTTask_ActivateAttack.h"
#include "AIController.h"
#include "Interface/MCOMonsterAIInterface.h"

UBTTask_ActivateAttack::UBTTask_ActivateAttack()
{
	NodeName = TEXT("Activate Attack");
}

EBTNodeResult::Type UBTTask_ActivateAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


	FMCOActionFinishedDelegate OnAttackFinished;
	OnAttackFinished.BindLambda([&](EBTNodeResult::Type bInResult)
	{
		FinishLatentTask(OwnerComp, bInResult);
	});

	// To finish this task after check hit
	AIPawn->SetActionFinishedDelegate(OnAttackFinished);

	// Check hit now
	AIPawn->OnActivateAction();

	return EBTNodeResult::InProgress;
}
