#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_ChargeStamina.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "Interface/MCOCharacterInterface.h"


UMCOGameplayAbility_ChargeStamina::UMCOGameplayAbility_ChargeStamina()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Character/DA_Character_ChargeStamina.DA_Character_ChargeStamina"));

	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Attribute* Stamina = Data->ActionDefinition->GetAttributeFragment();
	UpdateAttributeFragment(Stamina);
	
	SetID(EMCOAbilityID::Idle, Data->ActivationTag);
	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag);

	// This can be blocked by these tags
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DeadTag);
}

bool UMCOGameplayAbility_ChargeStamina::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	MCOLOG(TEXT("--------------------------CanActivate charge ability"));
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == true);
	
	const IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(nullptr != CharacterInterface);
	ISTRUE_F(true == CharacterInterface->CanChargeStamina());

	return true;
}

void UMCOGameplayAbility_ChargeStamina::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	MCOLOG(TEXT("--------------------------Activate charge ability"));

}

void UMCOGameplayAbility_ChargeStamina::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	MCOLOG(TEXT("--------------------------End charge ability"));
}

