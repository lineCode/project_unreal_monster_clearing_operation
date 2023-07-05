#include "MCOGameplayAbility_Equip.h"

#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Equip::UMCOGameplayAbility_Equip()
{
	GETASSET(MontageOnEquip, UAnimMontage, TEXT("/Game/Player/Animations/Montages/GreatSword_Equip_Montage.GreatSword_Equip_Montage"));
	GETASSET(MontageOnUnequip, UAnimMontage, TEXT("/Game/Player/Animations/Montages/GreatSword_Unequip_Montage.GreatSword_Unequip_Montage"));
	
	bIsToEquip = true;
	bAutoStopCharacter = true;	
}

// void UMCOGameplayAbility_Equip::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	SetID(EMCOAbilityID::Equip, FMCOCharacterTags::Get().EquipTag);
//
// 	// Blocked by these
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
// 	
// 	// Cancel these 
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
// }

void UMCOGameplayAbility_Equip::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(GetActor());
	ensure(PlayerInterface);

	bIsToEquip = false == PlayerInterface->IsEquipped();
	StartActivationWithMontageAndEventTag(bIsToEquip ? MontageOnEquip : MontageOnUnequip);
	
	PlayerInterface->SwitchEquipUnequip();
}

void UMCOGameplayAbility_Equip::OnGrantedEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(GetActor());
	ensure(PlayerInterface);

	if (EventTag == FMCOCharacterTags::Get().AnimNotify_Player_BeginEquip)
	{
		PlayerInterface->BeginAnimation_Equip();	
	}
	else if (EventTag == FMCOCharacterTags::Get().AnimNotify_Player_EndEquip)
	{
		PlayerInterface->EndAnimation_Equip();	
	}
}
