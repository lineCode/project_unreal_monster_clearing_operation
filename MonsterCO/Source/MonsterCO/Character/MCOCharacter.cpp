#include "MCOCharacter.h"
#include "MCOCharacterData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "UI/MCOHpWidget.h"
#include "UI/MCOAttributeWidget.h"
#include "MCOPlayerState.h"
#include "Item/MCOItemData.h"
#include "Item/MCOItemData_Potion.h"


AMCOCharacter::AMCOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// PrimaryActorTick.bCanEverTick = true;
	
	TakeItemActions.Emplace(EMCOItemType::Weapon, FTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &ThisClass::EquipWeapon)
	));
	TakeItemActions.Emplace(EMCOItemType::Potion, FTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &ThisClass::DrinkPotion)
	));
	TakeItemActions.Emplace(EMCOItemType::Scroll, FTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &ThisClass::ReadScroll)
	));
}

void AMCOCharacter::SetCharacterData()
{
	ensure(nullptr != CharacterData);
	
	bAlwaysRelevant = true;
	CharacterName = CharacterData->CharacterName;

	GetCharacterMovement()->RotationRate = CharacterData->RotationRate;

	// Set these in BP
	// GetCapsuleComponent()->SetHiddenInGame(false);
	// GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MCOCAPSULE);
	// GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterData->CapsuleHalfHeight);
	// GetCapsuleComponent()->SetCapsuleRadius(CharacterData->CapsuleRadius);
	
	// GetMesh()->SetSkeletalMesh(CharacterData->Mesh);
	// GetMesh()->SetAnimInstanceClass(CharacterData->AnimInstance);
	// GetMesh()->SetRelativeLocationAndRotation(CharacterData->MeshRelativeLocation, CharacterData->MeshRelativeRotation);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* AMCOCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UMCOAbilitySystemComponent* AMCOCharacter::GetMCOAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AMCOCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Initialize();
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMCOCharacter::Initialize()
{
	AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	ISTRUE(nullptr != MCOPlayerState);

	AbilitySystemComponent = MCOPlayerState->GetMCOAbilitySystemComponent();
	ISTRUE(nullptr != AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(MCOPlayerState, this);
	AbilitySystemComponent->SetTagMapCount(FMCOCharacterTags::Get().DeadTag, 0);
	AbilitySystemComponent->OnDamagedReceived.AddDynamic(this, &AMCOCharacter::ReceiveDamage);
	
	// give ability set
	ISTRUE(GetLocalRole() == ROLE_Authority);
	ISTRUE(false == AbilitySystemComponent->bCharacterAbilitySetGiven);
	ISTRUE(nullptr != CharacterData)
	ISTRUE(nullptr != CharacterData->AbilitySet)
	CharacterData->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), &AbilitySetHandles, nullptr);

	// set delegates
	MCOPlayerState->InitializeAbilityDelegates();
	AttributeSet = AbilitySystemComponent->GetSet<UMCOAttributeSet>();
	ISTRUE(nullptr != AttributeSet);
	
	MCOLOG(TEXT("%s : InitValues: Health = %f / %f, Stemina = %f / %f"),
		*CharacterName.ToString(),
		GetHealth(), GetMaxHealth(),
		GetStamina(), GetMaxStamina()
	);
}

bool AMCOCharacter::IsAlive() const
{
	return 0.0f < GetHealth();
}

float AMCOCharacter::GetCurrentStamina() const
{
	return GetStamina();
}

bool AMCOCharacter::CanChargeStamina() const
{
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);
	ISTRUE_F(GetStamina() < GetMaxStamina());

	return true;
}

FVector AMCOCharacter::GetSocketLocation(const FName& InSocketName)
{
	FVector SocketLocation;
	if (nullptr != GetMesh() && true == GetMesh()->DoesSocketExist(InSocketName))
	{
		SocketLocation = GetMesh()->GetSocketLocation(InSocketName);
	}
	return SocketLocation;
}

int32 AMCOCharacter::GetAbilityLevel(EMCOAbilityID InAbilityLevelID) const
{
	// TO DO
	return 1;
}

float AMCOCharacter::GetHealth() const
{
	if (true == AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AMCOCharacter::GetMaxHealth() const
{
	if (true == AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AMCOCharacter::GetStiffness() const
{
	if (true == AttributeSet.IsValid())
	{
		return AttributeSet->GetStiffness();
	}
	return 0.0f;
}

float AMCOCharacter::GetMaxStiffness() const
{
	if (true == AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStiffness();
	}
	return 0.0f;
}

float AMCOCharacter::GetStamina() const
{
	if (true == AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina();
	}
	return 0.0f;
}

float AMCOCharacter::GetMaxStamina() const
{
	if (true == AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.0f;
}

bool AMCOCharacter::HasTag(const FGameplayTag& InTag) const
{
	return GetTagCount(InTag) > 0;
}

int32 AMCOCharacter::GetTagCount(const FGameplayTag& InTag) const
{
	ISTRUE_Z(nullptr != GetAbilitySystemComponent());
	
	return GetAbilitySystemComponent()->GetTagCount(InTag);
}

void AMCOCharacter::ReceiveDamage(UMCOAbilitySystemComponent* SourceASC, float Damage)
{
	
}

void AMCOCharacter::SetDamagedData(const FMCODamagedData& InDamagedData)
{
	CurrentDamagedData = InDamagedData;
}

float AMCOCharacter::GetCapsuleRadius() const
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();

	return (nullptr != Capsule) ? Capsule->GetScaledCapsuleRadius() : 0.0f;
}

void AMCOCharacter::Die()
{
	MCOLOG(TEXT("%s : Died"), *CharacterName.ToString());

	if (nullptr != Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	StopCharacter(true);

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	ensure(nullptr != CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	ensure(nullptr != MovementComp);
	MovementComp->StopMovementImmediately();
	MovementComp->DisableMovement();

	
	
	// AbilitySetHandles.TakeFromAbilitySystem(GetMCOAbilitySystemComponent());
	
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetCharacterMovement()->GravityScale = 0;
	// GetCharacterMovement()->Velocity = FVector(0);
	
	
	// if (true == AbilitySystemComponent.IsValid())
	// {
	// 	AbilitySystemComponent->CancelAbilities();
	//
	// 	FGameplayTagContainer EffectsTagToRemove;
	// 	EffectsTagToRemove.AddTag(FMCOCharacterTags::Get().EffectRemoveOnDeathTag);
	// 	int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagToRemove);
	// 	AbilitySystemComponent->AddLooseGameplayTag(FMCOCharacterTags::Get().DeadTag);
	// }
	//
	// if (nullptr != DeathMontage)
	// {
	// 	PlayAnimMontage(DeathMontage);
	// }
	// else
	// {
	// 	FinishDying();
	// }
}

void AMCOCharacter::FinishDying()
{
	OnCharacterDeathFinished.Broadcast(this);
	
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}
	
	SetActorHiddenInGame(true);
	DestroyAllAttachedActors();
	
	// Uninitialize the ASC if we're still the avatar actor
	// (otherwise another pawn already did it when they became the avatar actor)
	ISTRUE(nullptr != AbilitySystemComponent.Get());
	ISTRUE(AbilitySystemComponent->GetAvatarActor() == GetOwner());
	
	AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
	AbilitySystemComponent->ClearAbilityInput();
	AbilitySystemComponent->RemoveAllGameplayCues();

	if (nullptr != AbilitySystemComponent->GetOwnerActor())
	{
		AbilitySystemComponent->SetAvatarActor(nullptr);
	}
	else
	{
		// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
		AbilitySystemComponent->ClearActorInfo();
	}

	AbilitySystemComponent = nullptr;
}

void AMCOCharacter::DestroyAllAttachedActors()
{
	TArray<AActor*> Actors;
	GetAttachedActors(Actors);

	for (AActor* Actor : Actors)
	{
		Actor->Destroy();
	}
}

void AMCOCharacter::StopCharacter(bool bToStop)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(CharacterMC);
	
	CharacterMC->MovementMode = (true == bToStop) ? MOVE_None : MOVE_Walking;
}

void AMCOCharacter::TakeItem(const UMCOItemData* InItemData)
{
	ensure(nullptr != InItemData);
	TakeItemActions[InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	
	FGameplayEventData Payload;
	Payload.EventTag       = FMCOCharacterTags::Get().GameplayEvent_TakeItemTag;
	Payload.Instigator     = this;
	Payload.Target         = this;
	GetAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);
}

void AMCOCharacter::DrinkPotion(const UMCOItemData* InItemData)
{
	const UMCOItemData_Potion* PotionData = Cast<UMCOItemData_Potion>(InItemData); 
	ensure(nullptr != PotionData);
	
	ItemAttributeFragment = PotionData->AttributeFragment;
}

void AMCOCharacter::EquipWeapon(const UMCOItemData* InItemData)
{
	MCOPRINT(TEXT("Equip Weapon"));
}

void AMCOCharacter::ReadScroll(const UMCOItemData* InItemData)
{
	MCOPRINT(TEXT("Read Scroll"));
}

UMCOActionFragment_Attribute* AMCOCharacter::GetItemAttributeFragment()
{
	return ItemAttributeFragment;
}

void AMCOCharacter::EndTakeItem()
{
	ItemAttributeFragment = nullptr;
}

void AMCOCharacter::InitializeWidget(UMCOHpWidget* InHpWidget, UMCOAttributeWidget* InAttributeWidget)
{
	ISTRUE(InHpWidget != nullptr);
	ISTRUE(InAttributeWidget != nullptr);
	
	InHpWidget->SetName(CharacterName.ToString());
	InHpWidget->SetMaxHp(GetMaxHealth());
	InHpWidget->UpdateHpBar(GetHealth());

	InAttributeWidget->UpdateHealth(GetHealth());
	InAttributeWidget->UpdateMaxHealth(GetMaxHealth());
	InAttributeWidget->UpdateStamina(GetStamina());
	InAttributeWidget->UpdateMaxStamina(GetMaxStamina());
	InAttributeWidget->UpdateStiffness(GetStiffness());
	InAttributeWidget->UpdateMaxStiffness(GetMaxStiffness());

	AMCOPlayerState* MCOPlayerState = Cast<AMCOPlayerState>(GetPlayerState());
	ISTRUE(MCOPlayerState);

	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetHealthAttribute().GetName(), InHpWidget, &UMCOHpWidget::UpdateHpBar
	);
	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetMaxHealthAttribute().GetName(), InHpWidget, &UMCOHpWidget::SetMaxHp
	);

	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetHealthAttribute().GetName(), InAttributeWidget, &UMCOAttributeWidget::UpdateHealth
	);
	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetMaxHealthAttribute().GetName(), InAttributeWidget, &UMCOAttributeWidget::UpdateMaxHealth
	);
	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetStaminaAttribute().GetName(), InAttributeWidget, &UMCOAttributeWidget::UpdateStamina
	);
	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetMaxStaminaAttribute().GetName(), InAttributeWidget, &UMCOAttributeWidget::UpdateMaxStamina
	);
	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetStiffnessAttribute().GetName(), InAttributeWidget, &UMCOAttributeWidget::UpdateStiffness
	);
	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetMaxStiffnessAttribute().GetName(), InAttributeWidget, &UMCOAttributeWidget::UpdateMaxStiffness
	);
}


// void AMCOCharacter::AddCharacterAbilities()
// {
// 	ISTRUE(GetLocalRole() == ROLE_Authority);
// 	ISTRUE(AbilitySystemComponent.IsValid() == true);
// 	ISTRUE(AbilitySystemComponent->bCharacterAbilitiesGiven == false);
//
// 	for (auto& GameplayAbility : CharacterData->CharacterAbilities)
// 	{
// 		FGameplayAbilitySpec Spec(
// 			GameplayAbility.Value,
// 			GetAbilityLevel(GameplayAbility.Value.GetDefaultObject()->AbilityLevelID),
// 			static_cast<int32>(GameplayAbility.Value.GetDefaultObject()->AbilityInputID), 
// 			this
// 		);
// 		
// 		Spec.DynamicAbilityTags.AppendTags(Spec.Ability->AbilityTags);
// 		AbilitySystemComponent->GiveAbility(Spec);
// 		
// 		MCOLOG(TEXT("Character Ability Added : %s"), *GameplayAbility.Value->GetName());
// 	}
//
// 	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
// }

// void AMCOCharacter::ClearCharacterAbilities()
// {
// 	ISTRUE(GetLocalRole() == ROLE_Authority);
// 	ISTRUE(AbilitySystemComponent.IsValid() == true);
// 	ISTRUE(AbilitySystemComponent->bCharacterAbilitiesGiven == true);
//
// 	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
// 	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
// 	{
// 		if (Spec.SourceObject == this && nullptr != AbilitySet->GrantedGameplayAbilities.Contains(Spec.Ability->GetClass()))
// 		{
// 			AbilitiesToRemove.Emplace(Spec.Handle);
// 		}
// 	}
//
// 	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
// 	{
// 		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
// 	}
//
// 	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
// }

// void AMCOCharacter::AddStartupEffects()
// {
// 	ISTRUE(GetLocalRole() == ROLE_Authority);
// 	ISTRUE(AbilitySystemComponent.IsValid() == true);
// 	ISTRUE(AbilitySystemComponent->bStartupEffectsApplied == false);
//
// 	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
// 	EffectContext.AddSourceObject(this);
//
// 	for (TSubclassOf<UGameplayEffect> GameplayEffect : CharacterData->StartupEffects)
// 	{
// 		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.0f, EffectContext);
// 		if (NewHandle.IsValid() == true)
// 		{
// 			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
// 		}
//
// 		MCOLOG(TEXT("Startup Effects added : %s"), *GameplayEffect->GetName());
// 	}
//
// 	AbilitySystemComponent->bStartupEffectsApplied = true;
//
// }

// void AMCOCharacter::InitializeDefaultAttributes()
// {
// 	ISTRUE(AttributeSetBase.IsValid() == true);
//
// 	if (CharacterData->DefaultAttributes == nullptr)
// 	{
// 		MCOLOG(TEXT("DefaultAttributes is nullptr"));
// 		return;
// 	}
//
// 	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
// 	EffectContext.AddSourceObject(this);
//
// 	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterData->DefaultAttributes, 1.0f, EffectContext);
// 	if (NewHandle.IsValid() == true)
// 	{
// 		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
// 	}
//
// 	MCOLOG(TEXT("DefaultAttributes added : %s"), *CharacterData->DefaultAttributes->GetName());
// }