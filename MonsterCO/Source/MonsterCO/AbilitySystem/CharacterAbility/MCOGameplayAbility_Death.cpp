#include "MCOGameplayAbility_Death.h"
#include "AbilitySystemComponent.h"
#include "Interface/MCOCharacterInterface.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"


UMCOGameplayAbility_Death::UMCOGameplayAbility_Death()
{	
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

// void UMCOGameplayAbility_Death::DoneAddingNativeTags()
// {	
// 	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_DeadTag);
// 	
// 	// Cancel ALL
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DodgeTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DamagedTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().TakeItemTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
// }

void UMCOGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterInterface);
	CharacterInterface->Die();
	
	CancelAllAbility();
	StartActivationWithMontage(Data->GetMontage(CharacterInterface->GetDamagedData().DamagedDegree));
}

void UMCOGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterInterface);
	CharacterInterface->FinishDying();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}