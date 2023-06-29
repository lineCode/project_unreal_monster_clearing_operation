#include "AbilitySystem/ActionData/MCOActionFragment_MonsterAI.h"
#include "Interface/MCOMonsterAIInterface.h"


void UMCOActionFragment_MonsterAI::OnActionFinished(AActor* InCurrentActor, const bool& InSucceeded) const
{
	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(InCurrentActor);
	ISTRUE(MonsterInterface);
	MonsterInterface->OnActionFinished(true == InSucceeded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UMCOActionFragment_MonsterAI::SetDamagedInBlackBoard(const AActor* InCurrentActor, const bool& InDamaged) const
{
	const IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(InCurrentActor);
	ISTRUE(MonsterInterface);
	MonsterInterface->SetDamagedInBlackBoard(InDamaged);
}

void UMCOActionFragment_MonsterAI::SetActivateActionDelegate(AActor* InCurrentActor, const FMCOActivateActionDelegate& InOnActivateAction) const
{
	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(InCurrentActor);
	ISTRUE(MonsterInterface);
	MonsterInterface->SetActivateActionDelegate(InOnActivateAction);
}
