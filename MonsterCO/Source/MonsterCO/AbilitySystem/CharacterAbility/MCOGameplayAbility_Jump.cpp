#include "MCOGameplayAbility_Jump.h"
#include "AbilitySystemComponent.h"
#include "Interface/MCOPlayerInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "Interface/MCOCharacterInterface.h"


UMCOGameplayAbility_Jump::UMCOGameplayAbility_Jump()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Player/CommonAction/DA_Player_Jump.DA_Player_Jump"));

	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Cooldown* Cooldown = Data->ActionDefinition->GetCooldownFragment();
	UpdateCooldownFragment(Cooldown);
	const UMCOActionFragment_Attribute* Stamina = Data->ActionDefinition->GetAttributeFragment();
	UpdateAttributeFragment(Stamina);
	
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

bool UMCOGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(PlayerInterface);
	ISTRUE_F(PlayerInterface->CanJumpAction());

	return true;
}

void UMCOGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(CharacterInterface);
	CharacterInterface->StopCharacter(true);
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->Jump();
}

void UMCOGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(ActorInfo != nullptr);
	ISTRUE(ActorInfo->AvatarActor != nullptr);
	
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
	
	if (0.0f < AbilityEndDelay && GetWorld()->GetTimerManager().GetTimerRemaining(AbilityEndDelayTimer) <= 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AbilityEndDelayTimer,
			this,
			&ThisClass::EndAbilityAfterDelay,
			AbilityEndDelay,
			false
		);
		return;
	}
	
	EndAbilityAfterDelay();
}

void UMCOGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		
	ActivateStaminaChargeAbility();
	MakeCharacterMove();
}

void UMCOGameplayAbility_Jump::EndAbilityAfterDelay()
{	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

