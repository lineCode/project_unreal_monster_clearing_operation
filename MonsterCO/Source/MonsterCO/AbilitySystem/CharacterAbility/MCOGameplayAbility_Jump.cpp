#include "MCOGameplayAbility_Jump.h"
#include "AbilitySystemComponent.h"
#include "Interface/MCOPlayerInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/ActionData/MCOActionData.h"


UMCOGameplayAbility_Jump::UMCOGameplayAbility_Jump()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Player/CommonAction/DA_Player_Jump.DA_Player_Jump"));
	Data->UpdateDefinition(CurrentDefinition);
}

// void UMCOGameplayAbility_Jump::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	SetID(EMCOAbilityID::Jump, Data->ActivationTag);
// 	
// 	// Cancel these 
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
//
// 	// Blocked by these
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);	
// }

void UMCOGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->Jump();
}

void UMCOGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(ActorInfo != nullptr);
	ISTRUE(ActorInfo->AvatarActor != nullptr);
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();

	StopCharacter(true);
	
	if (0.0f < AbilityEndDelayTime && GetWorld()->GetTimerManager().GetTimerRemaining(AbilityEndDelayTimer) <= 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AbilityEndDelayTimer,
			this,
			&ThisClass::EndAbilityAfterDelay,
			AbilityEndDelayTime,
			false
		);
		return;
	}
	
	EndAbilityAfterDelay();
}

void UMCOGameplayAbility_Jump::EndAbilityAfterDelay()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
	StopCharacter(false);
}

