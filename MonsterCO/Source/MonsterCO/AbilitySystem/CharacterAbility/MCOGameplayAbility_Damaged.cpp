#include "MCOGameplayAbility_Damaged.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "AbilitySystem/ActionData/MCOActionFragment_MonsterAI.h"
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

	if (nullptr != MonsterAIFragment)
	{
		MonsterAIFragment->SetDamagedInBlackBoard(CurrentActorInfo->AvatarActor.Get(), true);
	}
	
	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	if (nullptr != PlayerInterface)
	{
		PlayerInterface->EquipInstantly();
	}
	
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(CharacterInterface);
	
	CancelAllAbility();
	// CharacterInterface->OffAllCollision();

	const EMCOCharacterDirection Direction = Data->GetDirectionFromDegree(CharacterInterface->GetDamagedDegreeThenSetZero(), true);
	StartActivationWithMontage(Data->GetMontage(Direction));
}

void UMCOGameplayAbility_Damaged::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (nullptr != MonsterAIFragment)
	{
		MonsterAIFragment->SetDamagedInBlackBoard(CurrentActorInfo->AvatarActor.Get(), false);
	}
}
