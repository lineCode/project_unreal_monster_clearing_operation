#include "MCOGameplayAbility_Dash.h"
#include "MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Dash::UMCOGameplayAbility_Dash()
{
	AbilityInputID = EMCOAbilityID::Dash;
	AbilityTag = FMCOCharacterTags::Get().DashTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);

	// Tag required to activate this ability 
	ActivationRequiredTags.AddTag(FMCOCharacterTags::Get().GameplayEffect_AfterDodgeTag);

	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
}

bool UMCOGameplayAbility_Dash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == true);

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(nullptr != PlayerInterface);
	ISTRUE_F(true == PlayerInterface->CanDashAction());

	return true;
}

void UMCOGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ensure(PlayerInterface);
	PlayerInterface->SetSpeed(EMCOCharacterSpeed::Fast);
}

void UMCOGameplayAbility_Dash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(ActorInfo != nullptr);
	ISTRUE(ActorInfo->AvatarActor != nullptr);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMCOGameplayAbility_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(PlayerInterface);
	PlayerInterface->SetSpeed(EMCOCharacterSpeed::Normal);
}
