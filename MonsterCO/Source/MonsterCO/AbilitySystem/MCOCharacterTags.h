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
	FGameplayTag GameplayEffect_DamageTag;
	FGameplayTag GameplayEffect_StiffnessTag;
	FGameplayTag GameplayEffect_CooldownTag;
	FGameplayTag GameplayEffect_AfterDodgeTag;

	// Event
	FGameplayTag GameplayEvent_DamagedTag;
	FGameplayTag GameplayEvent_DeadTag;

	// Debuff
	FGameplayTag DamagedTag;
	FGameplayTag StunTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	// Player ability
	FGameplayTag JumpTag;
	FGameplayTag EquipTag;
	FGameplayTag DodgeTag;
	FGameplayTag DashTag;
	FGameplayTag AttackTag;

private:
	static FMCOCharacterTags CharacterTags;
};

