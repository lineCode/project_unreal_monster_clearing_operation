#include "MCOGameplayAbility_Equip.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MCOCharacterTags.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Equip::UMCOGameplayAbility_Equip()
{
	bIsToEquip = true;

	AbilityInputID = EMCOAbilityID::Equip;
	AbilityTag = FMCOCharacterTags::Get().EquipTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);

	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);

	GETASSET(MontageOnEquip, UAnimMontage, TEXT("/Game/Player/Animations/Montages/GreatSword_Equip_Montage.GreatSword_Equip_Montage"));
	GETASSET(MontageOnUnequip, UAnimMontage, TEXT("/Game/Player/Animations/Montages/GreatSword_Unequip_Montage.GreatSword_Unequip_Montage"));
	
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
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);

	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ensure(PlayerInterface);

	StartActivationWithMontage(PlayerInterface->IsEquipped() ? MontageOnUnequip : MontageOnEquip);
	
	CharacterInterface->ControlMoving(true);
	PlayerInterface->SwitchEquipUnequip();
}

void UMCOGameplayAbility_Equip::OnTaskCompleted()
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);
	CharacterInterface->ControlMoving(false);
	
	Super::OnTaskCompleted();
}

void UMCOGameplayAbility_Equip::OnTaskCancelled()
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);
	CharacterInterface->ControlMoving(false);
	
	Super::OnTaskCancelled();
}