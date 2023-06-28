#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_Invincible.h"


UMCOGameplayAbility_Invincible::UMCOGameplayAbility_Invincible()
{
}

void UMCOGameplayAbility_Invincible::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));


		
}
