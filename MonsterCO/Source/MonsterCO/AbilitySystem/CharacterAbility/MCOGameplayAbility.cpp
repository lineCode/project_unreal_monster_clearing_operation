#include "MCOGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Stamina.h"
#include "GameFramework/Character.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOHUDInterface.h"


UMCOGameplayAbility::UMCOGameplayAbility()
{
	// Effect 
	GETCLASS(TagEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_GiveAbilityTags.GE_GiveAbilityTags_C"));
	GETCLASS(CooldownEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Cooldown.GE_Cooldown_C"));
	GETCLASS(InstantStaminaEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Stamina_Instant.GE_Stamina_Instant_C"));
	GETCLASS(InfiniteStaminaEffectClass, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Stamina_Infinite.GE_Stamina_Infinite_C"));
	
	// Setting
	bActivateAbilityOnGranted = false;
	
	ActivationPolicy = EMCOAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// Tags that can't be cancelled
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DeadTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().StunTag);	
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
}

void UMCOGameplayAbility::SetID(const EMCOAbilityID& InAbilityID, const FGameplayTag& InActivationTag)
{
	AbilityInputID = InAbilityID; 
	AbilityTag = InActivationTag; 
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
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
	
	const IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(CharacterInterface);
	if (nullptr != StaminaFragment)
	{
		ISTRUE_F(CharacterInterface->CanActionWithStamina(StaminaFragment->StaminaUsage));
	}

	return true;
}

void UMCOGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	StopConsumeStamina();
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

	ConsumeStaminaByPolicy(Handle, ActorInfo, ActivationInfo);

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
	
	const IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != HUDInterface);
	HUDInterface->StartCooldownWidget(AbilityTag, CooldownFragment->CooldownTime);
}

void UMCOGameplayAbility::UpdateStaminaFragment(const UMCOActionFragment_Stamina* InStaminaFragment)
{
	StaminaFragment = InStaminaFragment;
}

void UMCOGameplayAbility::ConsumeStaminaByPolicy(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(nullptr != StaminaFragment);
	ISTRUE(true == StaminaFragment->CanConsumeStamina());

	if (StaminaFragment->StaminaConsumptionPolicy == EMCOStaminaConsumptionPolicy::Instant)
	{
		MCOLOG(TEXT("Instant effect"));
		ConsumeStamina(InstantStaminaEffectClass, Handle, ActorInfo, ActivationInfo);
	}
	else if (StaminaFragment->StaminaConsumptionPolicy == EMCOStaminaConsumptionPolicy::Infinite)
	{
		MCOLOG(TEXT("Infinite effect"));
		ConsumeStamina(InfiniteStaminaEffectClass, Handle, ActorInfo, ActivationInfo);
	}	
}

void UMCOGameplayAbility::ConsumeStamina(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	ISTRUE(nullptr != StaminaFragment);
	ISTRUE(nullptr != GameplayEffectClass);
	ISTRUE(true == StaminaFragment->CanConsumeStamina());

	MCOLOG(TEXT("Consume Stamina : %f"), StaminaFragment->StaminaUsage);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
		
	const FGameplayEffectSpecHandle HandleForStamina = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
	ISTRUE(true == HandleForStamina.IsValid());
	
	HandleForStamina.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_StaminaTag,
		-StaminaFragment->StaminaUsage
	);

	FGameplayTagContainer Tags;
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StaminaTag);
	HandleForStamina.Data->DynamicGrantedTags = Tags;
	
	const FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(
		*HandleForStamina.Data.Get(),
		ASC->GetPredictionKeyForNewAction()
	);
}

void UMCOGameplayAbility::StopConsumeStamina()
{
	ISTRUE(nullptr != StaminaFragment);
	ISTRUE(true == StaminaFragment->CanConsumeStamina());
	
	MCOLOG(TEXT("Stop Consuming Stamina"));
	
	FGameplayTagContainer Tags;
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StaminaTag);
	GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(Tags); // "Granted" tags 
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
