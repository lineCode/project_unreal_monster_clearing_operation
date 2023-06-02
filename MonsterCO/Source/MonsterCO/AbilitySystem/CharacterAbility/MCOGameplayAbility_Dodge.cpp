#include "MCOGameplayAbility_Dodge.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "MCOGameplayAbility_Dash.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Dodge::UMCOGameplayAbility_Dodge(const FObjectInitializer& ObjectInitializer)
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Player/CommonAction/DA_Player_Dodge.DA_Player_Dodge"));
	
	SetID(EMCOAbilityID::Dodge, Data->ActivationTag);
	
	ensure(nullptr != Data);
	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Cooldown* Fragment = Data->ActionDefinition->GetCooldownFragment();
	ensure(nullptr != Fragment);
	UpdateCooldownFragment(Fragment);
	
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().AttackTag);

	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);

	Strength = 500.0f;
	Duration = 0.5f;
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
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

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
