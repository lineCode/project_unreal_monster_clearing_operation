#include "MCOGameplayAbility_Dash.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOPlayerInterface.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"


UMCOGameplayAbility_Dash::UMCOGameplayAbility_Dash()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Player/TwohandAction/DA_Twohand_Dash.DA_Twohand_Dash"));

	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Attribute* Stamina = Data->ActionDefinition->GetAttributeFragment();
	UpdateAttributeFragment(Stamina);
	
	SetID(EMCOAbilityID::Dash, Data->ActivationTag);
	SetTriggerTag(FMCOCharacterTags::Get().GameplayEffect_AfterDodgeTag);

	// Tag required to activate this ability 
	// ActivationRequiredTags.AddTag(FMCOCharacterTags::Get().GameplayEffect_AfterDodgeTag);

	// This can be blocked by these tags
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	
	// Cancel these 
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
	
}

bool UMCOGameplayAbility_Dash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	MCOLOG_C(MCOAbility, TEXT("CanActivateAbility : Dash"));
	
	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(nullptr != PlayerInterface);
	ISTRUE_F(true == PlayerInterface->CanDashAction());

	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == true);

	return true;
}

void UMCOGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	MCOLOG_C(MCOAbility, TEXT("ActivateAbility : Dash"));
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ensure(PlayerInterface);
	PlayerInterface->SetSpeed(EMCOCharacterSpeed::Fast);
}

void UMCOGameplayAbility_Dash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	MCOLOG_C(MCOAbility, TEXT("InputReleased : Dash"));
	
	ISTRUE(ActorInfo != nullptr);
	ISTRUE(ActorInfo->AvatarActor != nullptr);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UMCOGameplayAbility_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	MCOLOG_C(MCOAbility, TEXT("End : Dash"));
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE(PlayerInterface);
	PlayerInterface->SetSpeed(EMCOCharacterSpeed::Normal);
	
	StartStaminaChargeTimer();

}
