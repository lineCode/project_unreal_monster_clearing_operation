#include "AbilitySystem/ActionData/MCOActionFragment_MonsterAI.h"
#include "Interface/MCOMonsterAIInterface.h"


void UMCOActionFragment_MonsterAI::OnActionFinished(const AActor* InCurrentActor, const bool& InSucceeded) const
{
	const IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(InCurrentActor);
	ensure(MonsterInterface);
	MonsterInterface->OnActionFinished(true == InSucceeded ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}
