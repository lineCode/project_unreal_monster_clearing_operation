#include "AI/BTTask_SetInvincible.h"
#include "AIController.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTTask_SetInvincible::UBTTask_SetInvincible()
{
	NodeName = TEXT("Invincible");
}

EBTNodeResult::Type UBTTask_SetInvincible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (true == bSetInvincible)
	{
		const bool bTryActivate = AIPawn->TryActivateAbilityByTag(FMCOCharacterTags::Get().CharacterAbility_Invincible);
		if (false == bTryActivate)
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		AIPawn->CancelAbilityByTag(FMCOCharacterTags::Get().CharacterAbility_Invincible);
	}
	
	return EBTNodeResult::Succeeded;
}
