#include "MCOGameplayAbility_Death.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOCharacterTags.h"
#include "Character/MCOCharacter.h"


UMCOGameplayAbility_Death::UMCOGameplayAbility_Death()
{
	AbilityInputID = EMCOAbilityID::Dead;
	AbilityTag = FMCOCharacterTags::Get().DeadTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FMCOCharacterTags::Get().GameplayEvent_DeadTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Emplace(TriggerData);
}

void UMCOGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));

	AMCOCharacter* MCOCharacter = Cast<AMCOCharacter>(GetMCOCharacter());
	ISTRUE(nullptr != MCOCharacter);
	MCOCharacter->Die();
	
	CancelAllAbility();
	StartActivationWithMontage(MontageToPlay);	
}

void UMCOGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	AMCOCharacter* MCOCharacter = Cast<AMCOCharacter>(GetMCOCharacter());
	ISTRUE(nullptr != MCOCharacter);
	MCOCharacter->FinishDying();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}