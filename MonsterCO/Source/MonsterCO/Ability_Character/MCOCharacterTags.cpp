#include "MCOCharacterTags.h"
#include "Containers/UnrealString.h"

FMCOCharacterTags FMCOCharacterTags::CharacterTags;
FMCOCharacterTags::FMCOCharacterTags()
{
	MoveTag = FGameplayTag::RequestGameplayTag(FName("Input.Move"));
	LookTag = FGameplayTag::RequestGameplayTag(FName("Input.Look"));
	
	GameplayEffect_DamageTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Damage"));
	GameplayEffect_StiffnessTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stiffness"));
	GameplayEffect_CooldownTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Cooldown"));
	GameplayEffect_AfterDodgeTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.AfterDodge"));
	
	GameplayEvent_DeadTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Dead"));
	GameplayEvent_DamagedTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Damaged"));
	
	DamagedTag = FGameplayTag::RequestGameplayTag(FName("State.Damaged"));
	StunTag = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"));
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
		
	JumpTag = FGameplayTag::RequestGameplayTag(FName("State.Jump"));
	EquipTag = FGameplayTag::RequestGameplayTag(FName("State.Equip"));
	DodgeTag = FGameplayTag::RequestGameplayTag(FName("State.Dodge"));
	DashTag = FGameplayTag::RequestGameplayTag(FName("State.Dash"));
	AttackTag = FGameplayTag::RequestGameplayTag(FName("State.Attack"));
}

