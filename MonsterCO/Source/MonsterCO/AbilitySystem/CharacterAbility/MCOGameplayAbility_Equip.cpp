#include "MCOGameplayAbility_Equip.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Equip::UMCOGameplayAbility_Equip()
{
	GETASSET(MontageOnEquip, UAnimMontage, TEXT("/Game/Player/Animations/Montages/GreatSword_Equip_Montage.GreatSword_Equip_Montage"));
	GETASSET(MontageOnUnequip, UAnimMontage, TEXT("/Game/Player/Animations/Montages/GreatSword_Unequip_Montage.GreatSword_Unequip_Montage"));
	
	SetID(EMCOAbilityID::Equip, FMCOCharacterTags::Get().EquipTag);

	// This can be blocked by these tags
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
	
	// Cancel these 
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
	
	bIsToEquip = true;
}

bool UMCOGameplayAbility_Equip::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(PlayerInterface);
	ISTRUE_F(PlayerInterface->CanEquipAction());

	return true;
}

void UMCOGameplayAbility_Equip::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);

	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ensure(PlayerInterface);

	StartActivationWithMontage(PlayerInterface->IsEquipped() ? MontageOnUnequip : MontageOnEquip);
	
	CharacterInterface->StopCharacter(true);
	PlayerInterface->SwitchEquipUnequip();
}

void UMCOGameplayAbility_Equip::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	MakeCharacterMove();
	ActivateStaminaChargeAbility();
}