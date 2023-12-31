#include "MCOGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttributeEffect.h"
#include "AbilitySystem/ActionData/MCOActionFragment_MonsterAI.h"
#include "GameFramework/Character.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"


UMCOGameplayAbility::UMCOGameplayAbility()
{
	// Effect 
	ConstructorHelpers::FClassFinder<UGameplayEffect> InstantRef(TEXT("/Game/AbilitySystem/Effects/GE_Attribute_Instant.GE_Attribute_Instant_C"));
	if (true == InstantRef.Succeeded())
	{
		EffectClasses.Emplace(EMCOEffectPolicy::Instant, InstantRef.Class);
	}
	ConstructorHelpers::FClassFinder<UGameplayEffect> DurationRef( TEXT("/Game/AbilitySystem/Effects/GE_Attribute_Duration.GE_Attribute_Duration_C"));
	if (true == DurationRef.Succeeded())
	{
		EffectClasses.Emplace(EMCOEffectPolicy::Duration, DurationRef.Class);
	}
	ConstructorHelpers::FClassFinder<UGameplayEffect> InfiniteRef(TEXT("/Game/AbilitySystem/Effects/GE_Attribute_Infinite.GE_Attribute_Infinite_C"));
	if (true == InfiniteRef.Succeeded())
	{
		EffectClasses.Emplace(EMCOEffectPolicy::Infinite, InfiniteRef.Class);
	}
	
	// Setting
	bActivateAbilityOnGranted = false;
	bApplyCooldownOnGranted = false;
	bAutoStopCharacter = false;
	bAutoActivateChargingStaminaAbility = true;
	
	ActivationPolicy = EMCOAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// UGameplayTagsManager::Get().CallOrRegister_OnDoneAddingNativeTagsDelegate(
	// 	FSimpleDelegate::CreateUObject(this, &ThisClass::DoneAddingNativeTags)
	// );	
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

	SetDefaultDefinition();
	
	if (true == bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
	
	if (true == bApplyCooldownOnGranted)
	{
		//MCOLOG_C(MCOAbility, TEXT("%s : Apply cooldown on granted"), *AbilityTag.GetTagName().ToString());
		ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	}
}

bool UMCOGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	//MCOLOG_C(MCOAbility, TEXT("---- [%s] Check CanActivate"), *AbilityTag.GetTagName().ToString());
	
	ISTRUE_F(nullptr != ActorInfo);
	ISTRUE_F(true == ActorInfo->AvatarActor.IsValid());
	ISTRUE_F(true == ActorInfo->AbilitySystemComponent.IsValid());
	ISTRUE_F(true == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));

	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE_F(nullptr != CharacterInterface);
	ISTRUE_F(true == CharacterInterface->CanActivateAbility(AbilityTag));
	ISTRUE_F(true == CheckCanActivateWithStamina());

	return true;
}

void UMCOGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//MCOLOG_C(MCOAbility, TEXT("---- [%s] End"), *AbilityTag.GetTagName().ToString());
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	StopCharacterFromMoving(false);
	StopCharacterFromTurning(false);

	CancelAbilityEffectsSelf();

	if (true == bAutoActivateChargingStaminaAbility)
	{
		ActivateStaminaChargeAbility();
	}
	
	if (nullptr != MonsterAIFragment)
	{
		MonsterAIFragment->OnActionFinished(GetActor(), false == bWasCancelled);
	}
}

void UMCOGameplayAbility::SetDefaultDefinition()
{
	if (nullptr == CurrentDefinition)
	{
		CurrentDefinition = NewObject<UMCOActionDefinition>(this);
	}
}

bool UMCOGameplayAbility::SetAndCommitAbility(bool bIsCanBeCancelled, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return false;
	}

	//MCOLOG_C(MCOAbility, TEXT("---- [%s] Activated"), *AbilityTag.GetTagName().ToString());

	ApplyAbilityEffectSelf(Handle, ActorInfo, ActivationInfo);

	if (true == bAutoStopCharacter)
	{
		StopCharacterFromMoving(true);
	}

	return true;
}

AActor* UMCOGameplayAbility::GetActor() const
{
	return CurrentActorInfo ? CurrentActorInfo->AvatarActor.Get() : nullptr;
}

ACharacter* UMCOGameplayAbility::GetCharacter() const
{
	return CurrentActorInfo ? Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

AController* UMCOGameplayAbility::GetController() const
{
	return CurrentActorInfo ? CurrentActorInfo->PlayerController.Get() : nullptr;
}

IMCOCharacterInterface* UMCOGameplayAbility::GetMCOCharacterInterface() const
{
	return CurrentActorInfo ? Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

UAbilitySystemComponent* UMCOGameplayAbility::GetAbilitySystemComponent() const
{
	return CurrentActorInfo ? CurrentActorInfo->AbilitySystemComponent.Get() : nullptr;
}

UMCOAbilitySystemComponent* UMCOGameplayAbility::GetMCOAbilitySystemComponent() const
{
	return CurrentActorInfo ? Cast<UMCOAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr;
}

bool UMCOGameplayAbility::IsPlayer() const
{
	return CurrentActorInfo ? nullptr != Cast<IMCOPlayerInterface>(CurrentActorInfo->AvatarActor.Get()) : false;
}

void UMCOGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	ISTRUE(nullptr != CurrentDefinition->CooldownFragment);
	ISTRUE(true == CurrentDefinition->CooldownFragment->CanApplyCooldown());
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
	
	const FGameplayEffectSpecHandle NewHandle = MakeOutgoingGameplayEffectSpec(EffectClasses[EMCOEffectPolicy::Duration]);
	ISTRUE(true == NewHandle.IsValid());

	CurrentDefinition->CooldownFragment->ApplyCooldownValue(NewHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(
		*NewHandle.Data.Get(),
		ASC->GetPredictionKeyForNewAction()
	);
	//ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, NewHandle);
	
	StartCooldownWidget();
}

const FGameplayTagContainer* UMCOGameplayAbility::GetCooldownTags() const
{
	ISTRUE_N(nullptr != CurrentDefinition->CooldownFragment);
	ISTRUE_N(true == CurrentDefinition->CooldownFragment->CanApplyCooldown());
	
	return CurrentDefinition->CooldownFragment->GetCooldownTags();	
}

void UMCOGameplayAbility::StartCooldownWidget() const
{
	ISTRUE(nullptr != CurrentActorInfo);
	ISTRUE(true == CurrentDefinition->CooldownFragment->CanApplyCooldown());
	
	// const IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(GetActor());
	// ISTRUE(nullptr != HUDInterface);
	// HUDInterface->StartCooldownWidget(AbilityTag, CooldownFragment->CooldownTime);
}

bool UMCOGameplayAbility::CheckCanActivateWithStamina() const
{
	const UMCOAbilitySystemComponent* MCOASC = GetMCOAbilitySystemComponent();
	ISTRUE_F(nullptr != MCOASC);
	
	if (nullptr != CurrentDefinition->AttributeFragment)
	{
		return CurrentDefinition->AttributeFragment->CanActivateAbilityWithStamina(MCOASC->GetStamina());
	}
	
	return true;
}

void UMCOGameplayAbility::ApplyAbilityEffectSelf(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->AttributeFragment);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	for (const EMCOEffectPolicy& Policy : TEnumRange<EMCOEffectPolicy>())
	{
		if (false == EffectClasses.Contains(Policy))
		{
			continue;
		}
		
		ApplyEffect(
			ASC,
			CurrentDefinition->AttributeFragment,
			Policy,
			EffectClasses[Policy]
		);
	}
}

void UMCOGameplayAbility::CancelAbilityEffectsSelf() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
		
	FGameplayTagContainer Tags;
	Tags.AddTag(EffectTag);
	ASC->RemoveActiveEffectsWithGrantedTags(Tags); // Should be "Granted" tags

	//MCOLOG_C(MCOAbility, TEXT("Attribute Effect : Removed"));
}

void UMCOGameplayAbility::ApplyCueSelf(FGameplayTag InTag) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
	
	ASC->ExecuteGameplayCue(InTag);
}

bool UMCOGameplayAbility::ApplyEffect(
	UAbilitySystemComponent* ASC,
	const UMCOActionFragment_AttributeEffect* AttributeFragment,
	const EMCOEffectPolicy& InPolicy,
	const TSubclassOf<UGameplayEffect>& EffectClass) const
{
	ISTRUE_F(nullptr != ASC);
	ISTRUE_F(nullptr != EffectClass);
	ISTRUE_F(nullptr != AttributeFragment);
	ISTRUE_F(true == AttributeFragment->IsEffectExistByPolicy(InPolicy));
	
	const FGameplayEffectSpecHandle NewHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
	ISTRUE_F(true == NewHandle.IsValid());

	AttributeFragment->ApplyAttributeAdditiveValue(InPolicy, NewHandle);

	// to cancel on end ability 
	NewHandle.Data->DynamicGrantedTags.AddTag(EffectTag);
	
	ASC->ApplyGameplayEffectSpecToSelf(
		*NewHandle.Data.Get(),
		ASC->GetPredictionKeyForNewAction()
	);

	return true;
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
	AbilityTypesToIgnore.AddTag(FMCOCharacterTags::Get().DeadTag);

	// Cancel all abilities and block others from starting.
	ASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
}

void UMCOGameplayAbility::HandleGameplayEventWithTag(const FGameplayTag& InTag)
{
	FGameplayEventData Payload;
	Payload.EventTag       = InTag; //FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag;
	Payload.Instigator     = GetActor();
	Payload.Target         = GetActor();
	// Payload.OptionalObject = EffectSpec.Def;
	// Payload.ContextHandle  = EffectSpec.GetEffectContext();
	// Payload.InstigatorTags = *EffectSpec.CapturedSourceTags.GetAggregatedTags();
	// Payload.TargetTags     = *EffectSpec.CapturedTargetTags.GetAggregatedTags();
	// Payload.EventMagnitude = Magnitude;

	GetAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);
}

void UMCOGameplayAbility::StopCharacterFromMoving(bool bStopMoving) const
{
	//MCOLOG_C(MCOAbility, TEXT("Stop Character : %s"), bStopMoving ? TEXT("STOP") : TEXT("MOVE"));
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE(CharacterInterface);
	CharacterInterface->StopCharacterFromMoving(bStopMoving);
}

void UMCOGameplayAbility::StopCharacterFromTurning(bool bStopTurning) const
{
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE(CharacterInterface);
	CharacterInterface->StopCharacterFromTurning(bStopTurning);
}

void UMCOGameplayAbility::StartActivationWithMontage(UAnimMontage* InMontage, const FName& SectionName)
{
	ensure(nullptr != InMontage);
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,         // UGameplayAbility* OwningAbility
		SectionName,  // FName TaskInstanceName
		InMontage,    // UAnimMontage* MontageToPlay
		1.0f          // float Rate = 1.f
		// FName StartSection = NAME_None
		// bool bStopWhenAbilityEnds = true
		// float AnimRootMotionTranslationScale = 1.f
		// float StartTimeSeconds = 0.f
	);
	
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnTaskCompleted);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnTaskCompleted);
	
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnTaskCancelled);
	
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnTaskInterrupted);
	
	Task->ReadyForActivation();
}

void UMCOGameplayAbility::StartActivationWithMontageAndEventTag(UAnimMontage* InMontage, const FName& SectionName)
{
	ensure(nullptr != InMontage);
	
	UMCOAbilityTask_PlayMontageAndWaitForEvent* Task = UMCOAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this,                     // UGameplayAbility * OwningAbility,
		NAME_None,                // FName TaskInstanceName, 
		InMontage,                // UAnimMontage * MontageToPlay,
		FGameplayTagContainer(),  // FGameplayTagContainer EventTags, 
		1.0f,                     // float Rate, 
		SectionName,              // FName StartSection, 
		true,                     // bool bStopWhenAbilityEnds, 
		0.0f                      // float AnimRootMotionTranslationScale
	);
	
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnTaskCompletedWithEventTag);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnTaskCompletedWithEventTag);
	
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnTaskCancelledWithEventTag);
	
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnTaskInterruptedWithEventTag);
	
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

void UMCOGameplayAbility::OnTaskInterrupted()
{
	// removed for combo attack 
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMCOGameplayAbility::OnTaskCompletedWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	OnTaskCompleted();
}

void UMCOGameplayAbility::OnTaskCancelledWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	OnTaskCancelled();
}

void UMCOGameplayAbility::OnTaskInterruptedWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	OnTaskInterrupted();
}

void UMCOGameplayAbility::OnGrantedEventTag(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}
