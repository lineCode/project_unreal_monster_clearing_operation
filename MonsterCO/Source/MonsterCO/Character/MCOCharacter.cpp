#include "MCOCharacter.h"
#include "MCOCharacterData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "UI/Widgets/MCOHpWidget.h"
#include "UI/Widgets/MCOAttributeWidget.h"
#include "MCOPlayerState.h"


AMCOCharacter::AMCOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// PrimaryActorTick.bCanEverTick = true;

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

void AMCOCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeCharacter();
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMCOCharacter::StopCharacter(bool bToStop)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(CharacterMC);
	
	CharacterMC->MovementMode = (true == bToStop) ? MOVE_None : MOVE_Walking;
}

void AMCOCharacter::DisableMovement() const
{
	// ignore input
	if (nullptr != Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	// stop and disable movement
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	ensure(nullptr != MovementComp);
	MovementComp->StopMovementImmediately();
	MovementComp->DisableMovement();

	// GetCharacterMovement()->GravityScale = 0;
	// GetCharacterMovement()->Velocity = FVector(0);
}

void AMCOCharacter::DisableAllCollision()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	ensure(nullptr != CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AMCOCharacter::DestroyAllAttachedActors() const
{
	TArray<AActor*> Actors;
	GetAttachedActors(Actors);

	for (AActor* Actor : Actors)
	{
		Actor->Destroy();
	}
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
void AMCOCharacter::InitializeCharacter()
{
	// get ASC
	AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	ensure(nullptr != MCOPlayerState);
	AbilitySystemComponent = MCOPlayerState->GetMCOAbilitySystemComponent();
	ensure(nullptr != AbilitySystemComponent);

	// init ASC
	AbilitySystemComponent->InitAbilityActorInfo(MCOPlayerState, this);
	AbilitySystemComponent->SetTagMapCount(FMCOCharacterTags::Get().DeadTag, 0);
	AbilitySystemComponent->OnDamagedReceived.AddUniqueDynamic(this, &AMCOCharacter::ReceiveDamage);
	
	// give ability
	ISTRUE(GetLocalRole() == ROLE_Authority);
	ISTRUE(nullptr != CharacterData);
	ISTRUE(nullptr != CharacterData->AbilitySet);
	if (false == AbilitySystemComponent->bCharacterAbilitySetGiven)
	{	
		CharacterData->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent.Get(), &AbilitySetHandles, nullptr);
	}
	else
	{
		CharacterData->AbilitySet->AddStartupEffects(AbilitySystemComponent.Get(), &AbilitySetHandles, nullptr);
	}
	
	// set delegates "after giving abilities" 
	MCOPlayerState->InitializeAbilitySystem();
	AttributeSet = AbilitySystemComponent->GetSet<UMCOAttributeSet>();
	
	MCOPRINT(TEXT("[Init: %s] Health (%.1f/%.1f), Stemina (%.1f/%.1f), Stiffness (%.1f/%.1f)"),
		*CharacterName.ToString(),
		GetHealth(), GetMaxHealth(),
		GetStamina(), GetMaxStamina(),
		GetStiffness(), GetMaxStiffness()
	);
}

void AMCOCharacter::UninitializeAbilitySystem()
{
	ISTRUE(nullptr != AbilitySystemComponent.Get());
	
	// Uninitialize the ASC if we're still the avatar actor
	// (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (nullptr != AbilitySystemComponent->GetOwnerActor())
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner,
			// we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}
	}
	
	AbilitySystemComponent = nullptr;
}

UAbilitySystemComponent* AMCOCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UMCOAbilitySystemComponent* AMCOCharacter::GetMCOAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AMCOCharacter::CanActivateAbility(const FGameplayTag& InTag)
{
	if (InTag == FMCOCharacterTags::Get().ChargingTag)
	{
		ISTRUE_F(true == IsAlive());
		ISTRUE_F(false == GetMovementComponent()->IsFalling());
		ISTRUE_F(GetStamina() < GetMaxStamina());
	}
	else if (InTag == FMCOCharacterTags::Get().DamagedTag)
	{
		ISTRUE_F(true == IsAlive());
	}

	return true;
}

bool AMCOCharacter::IsAlive() const
{
	return 0.0f < GetHealth();
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
	MCOPRINT(TEXT("%s : Died"), *CharacterName.ToString());

	DisableMovement();
	DisableAllCollision();	
	
	// if (true == AbilitySystemComponent.IsValid())
	// {
	// 	AbilitySystemComponent->CancelAbilities();
	//
	// 	FGameplayTagContainer EffectsTagToRemove;
	// 	EffectsTagToRemove.AddTag(FMCOCharacterTags::Get().EffectRemoveOnDeathTag);
	// 	int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagToRemove);
	// 	AbilitySystemComponent->AddLooseGameplayTag(FMCOCharacterTags::Get().DeadTag);
	// }
}

void AMCOCharacter::FinishDying()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		DestroyAllAttachedActors();
		SetLifeSpan(0.1f);
	}

	SetActorHiddenInGame(true);
	UninitializeAbilitySystem();
	
	OnCharacterDeathFinished.Broadcast(this);
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