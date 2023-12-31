#include "MCOGameplayAbility_Dodge.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "GameFramework/RootMotionSource.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility_Dodge::UMCOGameplayAbility_Dodge(const FObjectInitializer& ObjectInitializer)
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Player/CommonAction/DA_Player_Dodge.DA_Player_Dodge"));
	
	Strength = 500.0f;
	Duration = 0.5f;
	bAutoActivateChargingStaminaAbility = false;
}

void UMCOGameplayAbility_Dodge::SetDefaultDefinition()
{
	Super::SetDefaultDefinition();
	
	ensure(nullptr != Data);
	Data->UpdateDefinition(CurrentDefinition);
}

// void UMCOGameplayAbility_Dodge::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	// Cancel these
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
//
// 	// Blocked by these
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DashTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);	
// }


void UMCOGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	ensure(nullptr != PlayerInterface);
	const FVector WorldDirection = PlayerInterface->GetInputWorldDirection();	
	bIsDodgeForward = PlayerInterface->IsInputForward();

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
}

void UMCOGameplayAbility_Dodge::OnTaskFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
	if (true == bIsDodgeForward)
	{
		HandleGameplayEventWithTag(FMCOCharacterTags::Get().GameplayEvent_AfterDodgeTag);
	
		// FGameplayEffectSpecHandle HandleForDash = MakeOutgoingGameplayEffectSpec(TagEffectClass);
		// ISTRUE(true == HandleForDash.IsValid());
		//
		// FGameplayTagContainer Tags;
		// Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_AfterDodgeTag);
		// HandleForDash.Data->DynamicGrantedTags = Tags;
		//
		// ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, HandleForDash);
	}

	ActivateStaminaChargeAbility();
}
