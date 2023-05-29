#include "MCOGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOCharacterTags.h"
#include "GameFramework/Character.h"
#include "Player/MCOPlayerCharacter.h"


UMCOGameplayAbility::UMCOGameplayAbility()
{
	// Effect 
	GETCLASS(CooldownEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Cooldown.GE_Cooldown_C"));
	GETCLASS(TagEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_AbilityTags.GE_AbilityTags_C"));
	
	// Setting
	bActivateAbilityOnGranted = false;
	bCanBeCancelled = true;
	bApplyCooldown = false;

	ActivationPolicy = EMCOAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Tags that can't be cancelled
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DeadTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().StunTag);	
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
}

void UMCOGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted == true)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

bool UMCOGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{	
	ISTRUE_F(true == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));
	ISTRUE_F(nullptr != ActorInfo);
	ISTRUE_F(true == ActorInfo->AvatarActor.IsValid());

	return true;
}

bool UMCOGameplayAbility::SetAndCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ensure(nullptr != ActorInfo);
	
	SetCanBeCanceled(bCanBeCancelled);
		
	// if (bHasBlueprintActivate)
	// {
	// 	// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
	// 	K2_ActivateAbility();
	// 	return false;
	// }
	// else if (bHasBlueprintActivateFromEvent)
	// {
	// 	if (TriggerEventData)
	// 	{
	// 		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
	// 		K2_ActivateAbilityFromEvent(*TriggerEventData);
	// 	}
	// 	else
	// 	{
	// 		//UE_LOG(LogAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use Activate Ability instead of Activate Ability From Event."), *GetName());
	// 		bool bReplicateEndAbility = false;
	// 		bool bWasCancelled = true;
	// 		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// 	}
	// 	return false;
	// }
	
	if (false == CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return false;
	}

	return true;
}

ACharacter* UMCOGameplayAbility::GetCharacter() const
{
	return Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
}

AMCOCharacter* UMCOGameplayAbility::GetMCOCharacter() const
{
	return Cast<AMCOCharacter>(CurrentActorInfo->AvatarActor.Get());
}

AController* UMCOGameplayAbility::GetController() const
{
	AMCOCharacter* Character = GetMCOCharacter();
	ISTRUE_N(Character);

	return Cast<AController>(Character->GetController());
}

UMCOAbilitySystemComponent* UMCOGameplayAbility::GetMCOAbilitySystemComponent() const
{
	AMCOCharacter* Character = GetMCOCharacter();
	ISTRUE_N(Character);

	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	ISTRUE_N(ASC);

	return Cast<UMCOAbilitySystemComponent>(ASC);
}

void UMCOGameplayAbility::SetCooldownGameplayEffect(const float& InDuration, const FGameplayTagContainer& InTags)
{
	CooldownTimeMax = InDuration;
	CooldownTags = InTags;
}

void UMCOGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	ISTRUE(nullptr != CooldownEffectClass);
	ISTRUE(true == bApplyCooldown);
	ISTRUE(0.0f != CooldownTimeMax);
	ISTRUE(false == CooldownTags.IsEmpty());

	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
	
	//MCOPRINT(TEXT("Applied Cooldown: %s (%f)"), *FHelper::GetEnumDisplayName(TEXT("EMCOAbilityID"), (int32)AbilityInputID), CooldownTimeMax);
	
	FGameplayEffectSpecHandle HandleForCooldown = MakeOutgoingGameplayEffectSpec(CooldownEffectClass);
	ISTRUE(true == HandleForCooldown.IsValid());
	
	HandleForCooldown.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_CooldownTag,
		CooldownTimeMax
	);
	
	HandleForCooldown.Data->DynamicGrantedTags = CooldownTags;

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, HandleForCooldown);
}

const FGameplayTagContainer* UMCOGameplayAbility::GetCooldownTags() const
{
	return &CooldownTags;	
}

void UMCOGameplayAbility::CancelAllAbility()
{
	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	FGameplayTagContainer AbilityTypesToIgnore;
	// AbilityTypesToIgnore.AddTag(FMCOCharacterTags::Get().Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	ASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
}

void UMCOGameplayAbility::StartActivationWithMontage(UAnimMontage* InMontage, const FName& SectionName)
{
	ensure(nullptr != InMontage);
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, 
		SectionName,
		InMontage,
		1.0f
	);
	
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnTaskCompleted);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnTaskCompleted);
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnTaskCancelled);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnTaskCancelled);
	
	Task->ReadyForActivation();
}

void UMCOGameplayAbility::StartActivationWithMontageAndEventTag(UAnimMontage* InMontage, const FName& SectionName)
{
	ensure(nullptr != InMontage);
	
	SetCanBeCanceled(bCanBeCancelled);
	
	UMCOAbilityTask_PlayMontageAndWaitForEvent* Task = UMCOAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this,                               // UGameplayAbility * OwningAbility,
		NAME_None,                          // FName TaskInstanceName, 
		InMontage,                          // UAnimMontage * MontageToPlay,
		FGameplayTagContainer(),            // FGameplayTagContainer EventTags, 
		1.0f,                               // float Rate, 
		SectionName,                        // FName StartSection, 
		true,                               // bool bStopWhenAbilityEnds, 
		0.0f                                // float AnimRootMotionTranslationScale
	);
	
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnTaskCompletedWithEventTag);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnTaskCompletedWithEventTag);
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnTaskCancelledWithEventTag);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnTaskCancelledWithEventTag);
	Task->EventReceived.AddDynamic(this, &ThisClass::OnGrantedEventTag);
	
	Task->ReadyForActivation();
}

void UMCOGameplayAbility::JumpToMontageSection(const UAnimMontage* InMontage, const FName& SectionName) const
{
	const ACharacter* Character = GetMCOCharacter();
	ISTRUE(Character != nullptr);
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	ISTRUE(AnimInstance != nullptr);

	AnimInstance->Montage_JumpToSection(SectionName, InMontage);
}

void UMCOGameplayAbility::OnTaskCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMCOGameplayAbility::OnTaskCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMCOGameplayAbility::OnTaskCompletedWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMCOGameplayAbility::OnTaskCancelledWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMCOGameplayAbility::OnGrantedEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// if (EventTag == FMCOCharacterTags::Get().AttackHitCheckTag)
	// {
	// 	GetMCOAbilitySystemComponent()->RemoveLooseGameplayTag(FMCOCharacterTags::Get().AttackHitCheckTag);
	// 	//...
	// }
}

EMCOCharacterDirection UMCOGameplayAbility::GetDirectionFromDegree(const EMCOCharacterDirectionOption InDirectionOption, const float InDegree)
{
	switch(InDirectionOption)
	{
	case EMCOCharacterDirectionOption::FrontBack:
	{
		if (-90.0f < InDegree && InDegree < 90.0f)
		{
			return EMCOCharacterDirection::Front; 
		}
		else
		{
			return EMCOCharacterDirection::Back; 
		}
	}
	break;
	case EMCOCharacterDirectionOption::LeftRight:
	{
		if (InDegree < 0.0f)
		{
			return EMCOCharacterDirection::Left; 
		}
		else
		{
			return EMCOCharacterDirection::Right; 
		}
	}
	break;
	case EMCOCharacterDirectionOption::FourSide:
	{
		if (InDegree < 0.0f)
		{
			if (InDegree > -45.0f)       return EMCOCharacterDirection::Front;
			else if (InDegree > -135.0f)  return EMCOCharacterDirection::Left; 
			else                         return EMCOCharacterDirection::Back;
		}
		else
		{
			if (InDegree < 45.0f)        return EMCOCharacterDirection::Front;
			else if (InDegree < 135.0f)   return EMCOCharacterDirection::Right; 
			else                         return EMCOCharacterDirection::Back;
		}
	}
	break;
	case EMCOCharacterDirectionOption::SixSide:
	{
		// Left
		if (InDegree < 0.0f)
		{
			if (InDegree > -60.0f)         return EMCOCharacterDirection::Front_Left;
			else if (InDegree > -120.0f)   return EMCOCharacterDirection::Left; 
			else                           return EMCOCharacterDirection::Back_Left;
		}
		// Right
		else
		{
			if (InDegree < 60.0f)          return EMCOCharacterDirection::Front_Right;
			else if (InDegree < 120.0f)    return EMCOCharacterDirection::Right; 
			else                           return EMCOCharacterDirection::Back_Right;
		}
	}
	break;
	case EMCOCharacterDirectionOption::EightSide:
	{
		// Left
		if (InDegree < 0.0f)
		{
			if (InDegree > -30.0f)       return EMCOCharacterDirection::Front;
			else if (InDegree > -60.0f)  return EMCOCharacterDirection::Front_Left;
			else if (InDegree > -90.0f)  return EMCOCharacterDirection::Left; 
			else if (InDegree > -120.0f) return EMCOCharacterDirection::Back_Left;
			else                         return EMCOCharacterDirection::Back;
		}
		// Right
		else
		{
			if (InDegree < 30.0f)        return EMCOCharacterDirection::Front;
			else if (InDegree < 60.0f)   return EMCOCharacterDirection::Front_Right;
			else if (InDegree < 90.0f)   return EMCOCharacterDirection::Right; 
			else if (InDegree < 120.0f)  return EMCOCharacterDirection::Back_Right;
			else                         return EMCOCharacterDirection::Back;
		}
	}
	break;
	}
	
	return EMCOCharacterDirection::Front;
}

