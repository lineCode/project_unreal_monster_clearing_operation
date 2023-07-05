#include "MCOCharacterTags.h"
#include "Containers/UnrealString.h"
#include "GameplayEffectTypes.h"

FMCOCharacterTags FMCOCharacterTags::CharacterTags;

void FMCOCharacterTags::InitializeTags()
{
	const bool bErrorIfNotFound = true; //EGameplayTagEventType::NewOrRemoved;

	// MAKE SURE THERE IS NO TYPO !!!! 
	
	CharacterTags.MoveTag = FGameplayTag::RequestGameplayTag(FName("Input.Move"), bErrorIfNotFound);
	CharacterTags.LookTag = FGameplayTag::RequestGameplayTag(FName("Input.Look"), bErrorIfNotFound);
	CharacterTags.OptionTag = FGameplayTag::RequestGameplayTag(FName("Input.Option"), bErrorIfNotFound);

	CharacterTags.GameplayEffectTag              = FGameplayTag::RequestGameplayTag(FName("GameplayEffect"),              bErrorIfNotFound);

	CharacterTags.GameplayEffect_TakeItemTag     = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.TakeItem"),     bErrorIfNotFound);
	CharacterTags.GameplayEffect_AbilityTag      = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Ability"),      bErrorIfNotFound);

	CharacterTags.GameplayEffect_DamageTag       = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Damage"),       bErrorIfNotFound);
	CharacterTags.GameplayEffect_StaminaTag      = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stamina"),      bErrorIfNotFound);
	CharacterTags.GameplayEffect_StiffnessTag    = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Stiffness"),    bErrorIfNotFound);
	CharacterTags.GameplayEffect_HealthTag       = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Health"),       bErrorIfNotFound);

	CharacterTags.GameplayEffect_DurationTag     = FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Duration"),     bErrorIfNotFound);
	
	CharacterTags.GameplayEvent_AfterDodgeTag    = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.AfterDodge"),   bErrorIfNotFound);
	CharacterTags.GameplayEvent_TakeItemTag      = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TakeItem"),      bErrorIfNotFound);
	CharacterTags.GameplayEvent_StaminaChargeTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.StaminaCharge"), bErrorIfNotFound);
	CharacterTags.GameplayEvent_DeadTag          = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Dead"),          bErrorIfNotFound);
	CharacterTags.GameplayEvent_DamagedTag       = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Damaged"),       bErrorIfNotFound);
	
	CharacterTags.JumpTag       = FGameplayTag::RequestGameplayTag(FName("State.Jump"),     bErrorIfNotFound);
	CharacterTags.EquipTag      = FGameplayTag::RequestGameplayTag(FName("State.Equip"),    bErrorIfNotFound);
	CharacterTags.DodgeTag      = FGameplayTag::RequestGameplayTag(FName("State.Dodge"),    bErrorIfNotFound);
	CharacterTags.DashTag       = FGameplayTag::RequestGameplayTag(FName("State.Dash"),     bErrorIfNotFound);
	CharacterTags.AttackTag     = FGameplayTag::RequestGameplayTag(FName("State.Attack"),   bErrorIfNotFound);
	CharacterTags.DamagedTag    = FGameplayTag::RequestGameplayTag(FName("State.Damaged"),  bErrorIfNotFound);
	CharacterTags.DeadTag       = FGameplayTag::RequestGameplayTag(FName("State.Dead"),     bErrorIfNotFound);
	CharacterTags.TakeItemTag   = FGameplayTag::RequestGameplayTag(FName("State.TakeItem"), bErrorIfNotFound);
	CharacterTags.ChargingTag   = FGameplayTag::RequestGameplayTag(FName("State.Charging"), bErrorIfNotFound);
	CharacterTags.InvincibleTag = FGameplayTag::RequestGameplayTag(FName("State.Invincible"),   bErrorIfNotFound);

	CharacterTags.EffectRemoveOnDeathTag  = FGameplayTag::RequestGameplayTag(FName("Ability.RemoveOnDeath"),        bErrorIfNotFound);
	CharacterTags.CancelOnStaminaEmptyTag = FGameplayTag::RequestGameplayTag(FName("Ability.CancelOnStaminaEmpty"), bErrorIfNotFound);

	CharacterTags.AnimNotify_Player_BeginEquip = FGameplayTag::RequestGameplayTag(FName("AnimNotify.Player.BeginEquip"), bErrorIfNotFound);
	CharacterTags.AnimNotify_Player_EndEquip = FGameplayTag::RequestGameplayTag(FName("AnimNotify.Player.EndEquip"), bErrorIfNotFound);
	
	CharacterTags.DragonAbility_Claw       = FGameplayTag::RequestGameplayTag(FName("Ability.Dragon.Claw"),     bErrorIfNotFound);
	CharacterTags.DragonAbility_Fireball   = FGameplayTag::RequestGameplayTag(FName("Ability.Dragon.Fireball"), bErrorIfNotFound);
	CharacterTags.DragonAbility_Breath     = FGameplayTag::RequestGameplayTag(FName("Ability.Dragon.Breath"),   bErrorIfNotFound);
	CharacterTags.DragonAbility_Falldown   = FGameplayTag::RequestGameplayTag(FName("Ability.Dragon.Falldown"),   bErrorIfNotFound);
	
	CharacterTags.CharacterAbility_Invincible   = FGameplayTag::RequestGameplayTag(FName("Ability.Character.Invincible"),   bErrorIfNotFound);
}

