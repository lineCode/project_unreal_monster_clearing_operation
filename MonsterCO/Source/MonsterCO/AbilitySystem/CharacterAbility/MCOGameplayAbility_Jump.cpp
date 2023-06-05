#include "MCOGameplayAbility_Jump.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"



UMCOGameplayAbility_Jump::UMCOGameplayAbility_Jump()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Player/CommonAction/DA_Player_Jump.DA_Player_Jump"));

	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Cooldown* Cooldown = Data->ActionDefinition->GetCooldownFragment();
	UpdateCooldownFragment(Cooldown);
	const UMCOActionFragment_Stamina* Stamina = Data->ActionDefinition->GetStaminaFragment();
	UpdateStaminaFragment(Stamina);
	
	SetID(EMCOAbilityID::Jump, Data->ActivationTag);
	
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);

	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);

}

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

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->Jump();
}

void UMCOGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(ActorInfo != nullptr);
	ISTRUE(ActorInfo->AvatarActor != nullptr);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMCOGameplayAbility_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

// void UMCOGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
// {
// 	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
// 	
// 	//if (ScopeLockCount > 0)
// 	//{
// 	//	WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGDGA_CharacterJump::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
// 	//	return;
// 	//}
//
// 	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
// 	Character->StopJumping();
// }
//
