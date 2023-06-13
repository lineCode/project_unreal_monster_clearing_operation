#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_TakeItem.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"
#include "Interface/MCOCharacterItemInterface.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Attribute.h"


UMCOGameplayAbility_TakeItem::UMCOGameplayAbility_TakeItem()
{
	SetID(EMCOAbilityID::Idle, FMCOCharacterTags::Get().TakeItemTag);
	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_TakeItemTag);
	
	// Cancel these
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
}

bool UMCOGameplayAbility_TakeItem::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == true);
	
	IMCOCharacterItemInterface* CharacterItemInterface = Cast<IMCOCharacterItemInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(nullptr != CharacterItemInterface);
	ISTRUE_F(nullptr != CharacterItemInterface->GetItemAttributeFragment());

	return true;
}

void UMCOGameplayAbility_TakeItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	IMCOCharacterItemInterface* CharacterItemInterface = Cast<IMCOCharacterItemInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterItemInterface);
	
	UpdateAttributeFragment(CharacterItemInterface->GetItemAttributeFragment());
	
	MCOLOG(TEXT("[Item Picked up] Health:[%.1f], Stamina:[%.1f], Stiffness:[%.1f]"),
		CharacterItemInterface->GetItemAttributeFragment()->GetHealthAdditiveValue(),
		CharacterItemInterface->GetItemAttributeFragment()->GetStaminaAdditiveValue(),
		CharacterItemInterface->GetItemAttributeFragment()->GetStiffnessAdditiveValue()
	);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
}

void UMCOGameplayAbility_TakeItem::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	IMCOCharacterItemInterface* CharacterItemInterface = Cast<IMCOCharacterItemInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != CharacterItemInterface);
	CharacterItemInterface->EndTakeItem();

	// !!!!!!!!!!!!!!!!!!!!!!
	//ActivateStaminaChargeAbility();
}

