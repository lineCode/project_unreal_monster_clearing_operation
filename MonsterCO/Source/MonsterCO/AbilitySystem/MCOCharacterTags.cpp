#include "MCOCharacterTags.h"
#include "Containers/UnrealString.h"
#include "GameplayEffectTypes.h"

FMCOCharacterTags FMCOCharacterTags::CharacterTags;

FMCOCharacterTags::FMCOCharacterTags()
{
	const bool ErrorIfNotFount = EGameplayTagEventType::NewOrRemoved;
	
	MoveTag = FGameplayTag::RequestGameplayTag(FName("Input.Move"), ErrorIfNotFount);
	LookTag = FGameplayTag::RequestGameplayTag(FName("Input.Look"), ErrorIfNotFount);
	
	GameplayEffect_CooldownTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Cooldown"), ErrorIfNotFount);
	GameplayEffect_AfterDodgeTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.AfterDodge"), ErrorIfNotFount);
	
	GameplayEffect_DamageTag    = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Damage"), ErrorIfNotFount);
	GameplayEffect_StaminaTag   = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stamina"), ErrorIfNotFount);
	GameplayEffect_StiffnessTag = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stiffness"), ErrorIfNotFount);
	GameplayEffect_HealthTag    = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Health"), ErrorIfNotFount);
	
	GameplayEvent_StaminaChargeTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.StaminaCharge"), ErrorIfNotFount);
	GameplayEvent_DeadTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Dead"), ErrorIfNotFount);
	GameplayEvent_DamagedTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Damaged"), ErrorIfNotFount);
	
	DamagedTag = FGameplayTag::RequestGameplayTag(FName("State.Damaged"), ErrorIfNotFount);
	StunTag = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"), ErrorIfNotFount);
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"), ErrorIfNotFount);
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"), ErrorIfNotFount);
		
	ChargingTag = FGameplayTag::RequestGameplayTag(FName("State.Charging"), ErrorIfNotFount);
	JumpTag = FGameplayTag::RequestGameplayTag(FName("State.Jump"), ErrorIfNotFount);
	EquipTag = FGameplayTag::RequestGameplayTag(FName("State.Equip"), ErrorIfNotFount);
	DodgeTag = FGameplayTag::RequestGameplayTag(FName("State.Dodge"), ErrorIfNotFount);
	DashTag = FGameplayTag::RequestGameplayTag(FName("State.Dash"), ErrorIfNotFount);
	AttackTag = FGameplayTag::RequestGameplayTag(FName("State.Attack"), ErrorIfNotFount);
}

