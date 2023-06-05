#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_ChargeStamina.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"



UMCOGameplayAbility_ChargeStamina::UMCOGameplayAbility_ChargeStamina()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Character/DA_Character_ChargeStamina.DA_Character_ChargeStamina"));

	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Stamina* Stamina = Data->ActionDefinition->GetStaminaFragment();
	UpdateStaminaFragment(Stamina);
	
	SetID(EMCOAbilityID::Idle, Data->ActivationTag);
	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag);
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

