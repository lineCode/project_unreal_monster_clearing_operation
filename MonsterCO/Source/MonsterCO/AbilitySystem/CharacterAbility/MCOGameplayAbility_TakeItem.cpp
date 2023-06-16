#include "MCOGameplayAbility_TakeItem.h"
#include "Interface/MCOPlayerInterface.h"
#include "Interface/MCOCharacterItemInterface.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Attribute.h"


UMCOGameplayAbility_TakeItem::UMCOGameplayAbility_TakeItem()
{
	
}

// void UMCOGameplayAbility_TakeItem::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	SetID(EMCOAbilityID::Idle, FMCOCharacterTags::Get().TakeItemTag);
// 	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_TakeItemTag);
// 	
// 	// Cancel these
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
// }

void UMCOGameplayAbility_TakeItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	IMCOCharacterItemInterface* CharacterItemInterface = Cast<IMCOCharacterItemInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterItemInterface);
	
	UpdateAttributeFragment(CharacterItemInterface->GetItemAttributeFragment());
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
}

void UMCOGameplayAbility_TakeItem::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	IMCOCharacterItemInterface* CharacterItemInterface = Cast<IMCOCharacterItemInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterItemInterface);
	CharacterItemInterface->EndTakeItem();
}

