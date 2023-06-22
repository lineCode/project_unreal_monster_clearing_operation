#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_ChargeStamina.h"
#include "Interface/MCOPlayerInterface.h"
#include "AbilitySystem/ActionData/MCOActionData.h"


UMCOGameplayAbility_ChargeStamina::UMCOGameplayAbility_ChargeStamina()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Character/DA_Character_ChargeStamina.DA_Character_ChargeStamina"));
	
}

void UMCOGameplayAbility_ChargeStamina::SetDefaultDefinition()
{
	Super::SetDefaultDefinition();
	
	ensure(nullptr != Data);
	Data->UpdateDefinition(CurrentDefinition);	
}

// void UMCOGameplayAbility_ChargeStamina::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
//
// 	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag);
//
// 	// Blocked by these
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);	
// }


void UMCOGameplayAbility_ChargeStamina::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
}
