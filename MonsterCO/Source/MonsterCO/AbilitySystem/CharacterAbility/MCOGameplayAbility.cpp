#include "MCOGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Attribute.h"
#include "GameFramework/Character.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOHUDInterface.h"


UMCOGameplayAbility::UMCOGameplayAbility()
{
	// Effect 
	GETCLASS(TagEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_GiveAbilityTags.GE_GiveAbilityTags_C"));
	GETCLASS(CooldownEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Cooldown.GE_Cooldown_C"));
	GETCLASS(InstantAttributeEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Attribute_Instant.GE_Attribute_Instant_C"));
	GETCLASS(InfiniteAttributeEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Attribute_Infinite.GE_Attribute_Infinite_C"));
	
	// Setting
	bActivateAbilityOnGranted = false;
	
	ActivationPolicy = EMCOAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
		
	// Blocked by these
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DeadTag);
}

void UMCOGameplayAbility::SetID(const EMCOAbilityID& InAbilityID, const FGameplayTag& InActivationTag)
{
	AbilityInputID = InAbilityID; 
	AbilityTag = InActivationTag; 
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
}

void UMCOGameplayAbility::SetCancelOnStaminaEmptyTag()
{
	AbilityTags.AddTag(FMCOCharacterTags::Get().CancelOnStaminaEmptyTag);
	ActivationOwnedTags.AddTag(FMCOCharacterTags::Get().CancelOnStaminaEmptyTag);
}

void UMCOGameplayAbility::SetTriggerTag(const FGameplayTag& InTag)
{
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = InTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Emplace(TriggerData);
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
	ISTRUE_F(true == CheckCanActivateWithStamina());

	return true;
}

void UMCOGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	StopAttributeEffect();
}

bool UMCOGameplayAbility::SetAndCommitAbility(const bool bIsCanBeCancelled, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ensure(nullptr != ActorInfo);
	
	// Set can be cancelled
	SetCanBeCanceled(bIsCanBeCancelled);
	
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

	ApplyAttributeEffect(Handle, ActorInfo, ActivationInfo);

	return true;
}

ACharacter* UMCOGameplayAbility::GetCharacter() const
{
	ISTRUE_N(CurrentActorInfo);
	
	return Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
}

AController* UMCOGameplayAbility::GetController() const
{
	const ACharacter* Character = GetCharacter();
	ISTRUE_N(Character);
	return Cast<AController>(Character->GetController());
}

UAbilitySystemComponent* UMCOGameplayAbility::GetAbilitySystemComponent() const
{
	ACharacter* Character = GetCharacter();
	ISTRUE_N(Character);
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Character);
	ISTRUE_N(ASCInterface);
	return ASCInterface->GetAbilitySystemComponent();
}

void UMCOGameplayAbility::UpdateCooldownFragment(const UMCOActionFragment_Cooldown* InCooldownFragment)
{
	CooldownFragment = InCooldownFragment;
}

void UMCOGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	ISTRUE(nullptr != CooldownFragment);
	ISTRUE(nullptr != CooldownEffectClass);
	ISTRUE(true == CooldownFragment->CanApplyCooldown());
	
	//MCOPRINT(TEXT("Applied Cooldown: %s (%f)"), *FHelper::GetEnumDisplayName(TEXT("EMCOAbilityID"), (int32)AbilityInputID), CooldownTimeMax);
	
	const FGameplayEffectSpecHandle HandleForCooldown = MakeOutgoingGameplayEffectSpec(CooldownEffectClass);
	ISTRUE(true == HandleForCooldown.IsValid());
	
	HandleForCooldown.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_CooldownTag,
		CooldownFragment->CooldownTime
	);
	
	HandleForCooldown.Data->DynamicGrantedTags = CooldownFragment->CooldownTags;

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, HandleForCooldown);

	StartCooldownWidget();
}

const FGameplayTagContainer* UMCOGameplayAbility::GetCooldownTags() const
{
	ISTRUE_N(nullptr != CooldownFragment);
	ISTRUE_N(nullptr != CooldownEffectClass);
	ISTRUE_N(true == CooldownFragment->CanApplyCooldown());
	
	return &CooldownFragment->CooldownTags;	
}

void UMCOGameplayAbility::StartCooldownWidget() const
{
	ISTRUE(nullptr != CurrentActorInfo);
	ISTRUE(true == CooldownFragment->CanApplyCooldown());
	
	// const IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(CurrentActorInfo->AvatarActor.Get());
	// ISTRUE(nullptr != HUDInterface);
	// HUDInterface->StartCooldownWidget(AbilityTag, CooldownFragment->CooldownTime);
}

void UMCOGameplayAbility::UpdateAttributeFragment(const UMCOActionFragment_Attribute* InAttributeFragment)
{
	AttributeFragment = InAttributeFragment;
}

bool UMCOGameplayAbility::CheckCanActivateWithStamina() const
{
	const IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE_F(CharacterInterface);
	if (nullptr != AttributeFragment)
	{
		ISTRUE_F(CharacterInterface->GetCurrentStamina() + AttributeFragment->GetStaminaAdditiveValue() >= 0.0f);
	}
	
	return true;
}

void UMCOGameplayAbility::ApplyAttributeEffect(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(nullptr != AttributeFragment);
	ISTRUE(nullptr != InstantAttributeEffectClass);
	ISTRUE(nullptr != InfiniteAttributeEffectClass);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
	
	const FGameplayEffectSpecHandle NewInstantHandle = MakeOutgoingGameplayEffectSpec(InstantAttributeEffectClass);
	ISTRUE(true == NewInstantHandle.IsValid());
	
	const FGameplayEffectSpecHandle NewInfiniteHandle = MakeOutgoingGameplayEffectSpec(InfiniteAttributeEffectClass);
	ISTRUE(true == NewInfiniteHandle.IsValid());

	AttributeFragment->ApplyAttributeAdditiveValue(
		NewInstantHandle, NewInfiniteHandle,
		FMCOCharacterTags::Get().GameplayEffect_HealthTag
	);

	AttributeFragment->ApplyAttributeAdditiveValue(
		NewInstantHandle, NewInfiniteHandle,
		FMCOCharacterTags::Get().GameplayEffect_StaminaTag
	);
	
	AttributeFragment->ApplyAttributeAdditiveValue(
		NewInstantHandle, NewInfiniteHandle,
		FMCOCharacterTags::Get().GameplayEffect_StiffnessTag
	);


	if (NewInstantHandle.Data->DynamicGrantedTags.Num() > 0)
	{
		ASC->ApplyGameplayEffectSpecToSelf(
			*NewInstantHandle.Data.Get(),
			ASC->GetPredictionKeyForNewAction()
		);
	}
	if (NewInfiniteHandle.Data->DynamicGrantedTags.Num() > 0)
	{
		ASC->ApplyGameplayEffectSpecToSelf(
			*NewInfiniteHandle.Data.Get(),
			ASC->GetPredictionKeyForNewAction()
		);
	}
}

void UMCOGameplayAbility::StopAttributeEffect()
{
	ISTRUE(nullptr != AttributeFragment);
		
	FGameplayTagContainer Tags;
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_HealthTag);
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag);
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StaminaTag);

	GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(Tags); // "Granted" tags
}

void UMCOGameplayAbility::ActivateStaminaChargeAbility()
{
	HandleGameplayEventWithTag(FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag);
}

void UMCOGameplayAbility::CancelAllAbility()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	FGameplayTagContainer AbilityTypesToIgnore;
	// AbilityTypesToIgnore.AddTag(FMCOCharacterTags::Get().Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	ASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
}

void UMCOGameplayAbility::HandleGameplayEventWithTag(const FGameplayTag& InTag)
{
	FGameplayEventData Payload;
	Payload.EventTag       = InTag; //FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag;
	Payload.Instigator     = CurrentActorInfo->AvatarActor.Get();
	Payload.Target         = CurrentActorInfo->AvatarActor.Get();
	// Payload.OptionalObject = EffectSpec.Def;
	// Payload.ContextHandle  = EffectSpec.GetEffectContext();
	// Payload.InstigatorTags = *EffectSpec.CapturedSourceTags.GetAggregatedTags();
	// Payload.TargetTags     = *EffectSpec.CapturedTargetTags.GetAggregatedTags();
	// Payload.EventMagnitude = Magnitude;

	GetAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);
}

void UMCOGameplayAbility::MakeCharacterMove() const
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(CharacterInterface);
	CharacterInterface->StopCharacter(false);
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
	const ACharacter* Character = GetCharacter();
	ISTRUE(Character != nullptr);
	ISTRUE(Character->GetMesh() != nullptr);
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
