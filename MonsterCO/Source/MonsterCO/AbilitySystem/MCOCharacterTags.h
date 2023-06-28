#pragma once

#include "GameplayTagContainer.h"


struct FMCOCharacterTags
{
public:
	static const FMCOCharacterTags& Get() { return CharacterTags; }

	static void InitializeTags();

	// Native ability
	FGameplayTag MoveTag;
	FGameplayTag LookTag;
	FGameplayTag OptionTag;

	// Effect
	FGameplayTag GameplayEffectTag;
	
	// Effect for ability begin ~ end 
	FGameplayTag GameplayEffect_TakeItemTag;
	FGameplayTag GameplayEffect_AbilityTag;

	// Effect for giving
	FGameplayTag GameplayEffect_DamageTag;
	FGameplayTag GameplayEffect_StiffnessTag;
	FGameplayTag GameplayEffect_HealthTag;
	FGameplayTag GameplayEffect_StaminaTag;

	// Effect for setting 
	FGameplayTag GameplayEffect_DurationTag;

	// Event
	FGameplayTag GameplayEvent_AfterDodgeTag;
	FGameplayTag GameplayEvent_TakeItemTag;
	FGameplayTag GameplayEvent_StaminaChargeTag;
	FGameplayTag GameplayEvent_DamagedTag;
	FGameplayTag GameplayEvent_DeadTag;
	
	// State
	FGameplayTag JumpTag;
	FGameplayTag EquipTag;
	FGameplayTag DodgeTag;
	FGameplayTag DashTag;
	FGameplayTag AttackTag;
	FGameplayTag TakeItemTag;
	FGameplayTag ChargingTag;
	FGameplayTag DamagedTag;
	FGameplayTag DeadTag;
	
	// Ability 
	FGameplayTag EffectRemoveOnDeathTag;
	FGameplayTag CancelOnStaminaEmptyTag;
	
	// Dragon
	FGameplayTag DragonAbility_Claw;
	FGameplayTag DragonAbility_Fireball;
	FGameplayTag DragonAbility_Breath;
	FGameplayTag DragonAbility_Falldown;

	// Character
	FGameplayTag CharacterAbility_Invincible;

private:
	static FMCOCharacterTags CharacterTags;
};

