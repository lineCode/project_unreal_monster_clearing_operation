#include "BTTask_Attack.h"
#include "AIController.h"
#include "MCOAIKeys.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MCOCharacterInterface.h"
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
	
	FMCOAICharacterTaskFinishedDelegate OnAttackFinished;
	OnAttackFinished.BindLambda([&](EBTNodeResult::Type bInResult)
	{
		FinishLatentTask(OwnerComp, bInResult);
		//MCOLOG_C(MCOMonsterAI, TEXT("Monster Attack [%s] is %s"), *AttackTag.GetTagName().ToString(),
			//(bInResult == EBTNodeResult::Failed) ? TEXT("FAILED") : TEXT("SUCCEEDED"));
	});

	AIPawn->SetActionDelegate(OnAttackFinished);
	
	const bool bTryAttack = AIPawn->Attack(AttackTag);

	if (false == bTryAttack)
	{
		return EBTNodeResult::Failed;
	}
	
	//MCOLOG_C(MCOMonsterAI, TEXT("Monster Attack [%s] is INPROGRESS"), *AttackTag.GetTagName().ToString());
	return EBTNodeResult::InProgress;
}
