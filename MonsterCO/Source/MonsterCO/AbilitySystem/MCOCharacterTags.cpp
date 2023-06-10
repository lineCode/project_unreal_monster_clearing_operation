#include "MCOCharacterTags.h"
#include "Containers/UnrealString.h"
#include "GameplayEffectTypes.h"

FMCOCharacterTags FMCOCharacterTags::CharacterTags;

FMCOCharacterTags::FMCOCharacterTags()
{
	const bool bErrorIfNotFound = EGameplayTagEventType::NewOrRemoved;
	
	MoveTag = FGameplayTag::RequestGameplayTag(FName("Input.Move"), bErrorIfNotFound);
	LookTag = FGameplayTag::RequestGameplayTag(FName("Input.Look"), bErrorIfNotFound);
	
	GameplayEffect_CooldownTag     = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Cooldown"),     bErrorIfNotFound);
	GameplayEffect_AfterDodgeTag   = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.AfterDodge"),   bErrorIfNotFound);
	  
	GameplayEffect_DamageTag       = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Damage"),       bErrorIfNotFound);
	GameplayEffect_StaminaTag      = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stamina"),      bErrorIfNotFound);
	GameplayEffect_StiffnessTag    = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stiffness"),    bErrorIfNotFound);
	GameplayEffect_HealthTag       = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Health"),       bErrorIfNotFound);
	
	GameplayEvent_TakeItemTag      = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TakeItem"),      bErrorIfNotFound);
	GameplayEvent_StaminaChargeTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.StaminaCharge"), bErrorIfNotFound);
	GameplayEvent_DeadTag          = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Dead"),          bErrorIfNotFound);
	GameplayEvent_DamagedTag       = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Damaged"),       bErrorIfNotFound);
	
	DamagedTag    = FGameplayTag::RequestGameplayTag(FName("State.Damaged"),     bErrorIfNotFound);
	StunTag       = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"), bErrorIfNotFound);
	DeadTag       = FGameplayTag::RequestGameplayTag(FName("State.Dead"),        bErrorIfNotFound);
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"), bErrorIfNotFound);
		
	CancelOnStaminaEmptyTag = FGameplayTag::RequestGameplayTag(FName("State.CancelOnStaminaEmpty"), bErrorIfNotFound);
	TakeItemTag   = FGameplayTag::RequestGameplayTag(FName("State.TakeItem"), bErrorIfNotFound);
	ChargingTag   = FGameplayTag::RequestGameplayTag(FName("State.Charging"), bErrorIfNotFound);
	JumpTag       = FGameplayTag::RequestGameplayTag(FName("State.Jump"),     bErrorIfNotFound);
	EquipTag      = FGameplayTag::RequestGameplayTag(FName("State.Equip"),    bErrorIfNotFound);
	DodgeTag      = FGameplayTag::RequestGameplayTag(FName("State.Dodge"),    bErrorIfNotFound);
	DashTag       = FGameplayTag::RequestGameplayTag(FName("State.Dash"),     bErrorIfNotFound);
	AttackTag     = FGameplayTag::RequestGameplayTag(FName("State.Attack"),   bErrorIfNotFound);
}

