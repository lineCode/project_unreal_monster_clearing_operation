#pragma once

#include "GameplayTagContainer.h"


struct FMCOCharacterTags
{
public:
	static const FMCOCharacterTags& Get() { return CharacterTags; }

	FMCOCharacterTags();

	// Native ability
	FGameplayTag MoveTag;
	FGameplayTag LookTag;

	// Effect
	FGameplayTag GameplayEffect_CooldownTag;
	FGameplayTag GameplayEffect_AfterDodgeTag;
	
	FGameplayTag GameplayEffect_DamageTag;
	FGameplayTag GameplayEffect_StiffnessTag;
	FGameplayTag GameplayEffect_HealthTag;
	FGameplayTag GameplayEffect_StaminaTag;

	// Event
	FGameplayTag GameplayEvent_TakeItemTag;
	FGameplayTag GameplayEvent_StaminaChargeTag;
	FGameplayTag GameplayEvent_DamagedTag;
	FGameplayTag GameplayEvent_DeadTag;

	// Cue
	

	// Ability 
	FGameplayTag EffectRemoveOnDeathTag;
	FGameplayTag CancelOnStaminaEmptyTag;
	
	// Debuff
	FGameplayTag DamagedTag;
	FGameplayTag DeadTag;

	// Action
	FGameplayTag TakeItemTag;
	FGameplayTag ChargingTag;
	
	// State
	FGameplayTag JumpTag;
	FGameplayTag EquipTag;
	FGameplayTag DodgeTag;
	FGameplayTag DashTag;
	FGameplayTag AttackTag;

private:
	static FMCOCharacterTags CharacterTags;
};

