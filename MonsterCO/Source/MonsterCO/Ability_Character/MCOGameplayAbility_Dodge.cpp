#include "MCOGameplayAbility_Dodge.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOCharacterTags.h"
#include "MCOGameplayAbility_Dash.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Dodge::UMCOGameplayAbility_Dodge(const FObjectInitializer& ObjectInitializer)
{
	AbilityInputID = EMCOAbilityID::Dodge;
	AbilityTag = FMCOCharacterTags::Get().DodgeTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);

	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().AttackTag);

	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
	
	bApplyCooldown = true;
	Strength = 500.0f;
	Duration = 0.5f;
	CooldownTimeMax = 3.0f;
}

bool UMCOGameplayAbility_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(PlayerInterface);
	ISTRUE_F(PlayerInterface->CanDodgeAction());

	return true;
}

void UMCOGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ensure(nullptr != PlayerInterface);
	const FVector WorldDirection = PlayerInterface->GetInputDirection();	
	bIsDodgeForward = PlayerInterface->IsDashForward();

	UAbilityTask_ApplyRootMotionConstantForce* Task = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce
	(
		this,
		NAME_None,
		WorldDirection,
		Strength,
		Duration,
		false,
		nullptr,
		ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
		FVector::ZeroVector,
		0.0f,
		true
	);

	Task->OnFinish.AddDynamic(this, &UMCOGameplayAbility_Dodge::OnTaskFinished);
	Task->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMCOGameplayAbility_Dodge::OnTaskFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	if (true == bIsDodgeForward)
	{
		FGameplayEffectSpecHandle HandleForDash = MakeOutgoingGameplayEffectSpec(TagEffectClass);
		ISTRUE(true == HandleForDash.IsValid());
	
		FGameplayTagContainer AfterDodgeTag;
		AfterDodgeTag.AddTag(FMCOCharacterTags::Get().GameplayEffect_AfterDodgeTag);
		HandleForDash.Data->DynamicGrantedTags = AfterDodgeTag;

		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, HandleForDash);
	}
}
