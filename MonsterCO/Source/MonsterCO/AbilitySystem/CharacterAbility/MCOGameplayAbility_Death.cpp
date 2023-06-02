#include "MCOGameplayAbility_Death.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOCharacterInterface.h"


UMCOGameplayAbility_Death::UMCOGameplayAbility_Death()
{
	SetID(EMCOAbilityID::Dead, FMCOCharacterTags::Get().DeadTag);
	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_DeadTag);
	
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UMCOGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterInterface);
	CharacterInterface->Die();
	
	CancelAllAbility();
	StartActivationWithMontage(MontageToPlay);	
}

void UMCOGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterInterface);
	CharacterInterface->FinishDying();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}