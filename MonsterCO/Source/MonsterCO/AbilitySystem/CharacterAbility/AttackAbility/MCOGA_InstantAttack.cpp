#include "AbilitySystem/CharacterAbility/AttackAbility/MCOGA_InstantAttack.h"
#include "AbilitySystem/ActionData/MCOActionData.h"


UMCOGA_InstantAttack::UMCOGA_InstantAttack()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Monster/Dragon/Action/DA_Dragon_Falldown.DA_Dragon_Falldown"));
}

void UMCOGA_InstantAttack::SetDefaultDefinition()
{
	Super::SetDefaultDefinition();
	
	ensure(nullptr != Data);
	Data->UpdateDefinition(CurrentDefinition);
}

void UMCOGA_InstantAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ensure(nullptr != Data);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	bIsInstantAttack = true;
	
	StartAttackActivation(
		nullptr,
		NAME_None
	);
}
