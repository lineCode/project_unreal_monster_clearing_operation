#include "MCOGameplayAbility_Damaged.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"



UMCOGameplayAbility_Damaged::UMCOGameplayAbility_Damaged()
{
	bAutoStopCharacter = true;
}

// void UMCOGameplayAbility_Damaged::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_DamagedTag);
//
// 	// Cancel these
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DodgeTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
// }

void UMCOGameplayAbility_Damaged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	if (nullptr != PlayerInterface)
	{
		PlayerInterface->SetEquippedWithoutAnimation();
	}
	
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(CharacterInterface);
	
	CancelAllAbility();
	// CharacterInterface->OffAllCollision();
	StartActivationWithMontage(Data->GetMontage(CharacterInterface->GetDamagedData().DamagedDegree));
}
