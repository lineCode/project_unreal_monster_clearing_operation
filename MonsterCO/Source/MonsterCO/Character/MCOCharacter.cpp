#include "MCOCharacter.h"
#include "MCOCharacterData.h"
#include "MCOPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "UI/SubWidgets/MCOHpWidget.h"
#include "UI/SubWidgets/MCOAttributeWidget.h"
#include "MCOPlayerState.h"
#include "CharacterAttachment/MCOModeComponent.h"
#include "CharacterAttachment/Attachment/MCOAttachment.h"
#include "Projectile/MCOProjectileSpawnComponent.h"


AMCOCharacter::AMCOCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// PrimaryActorTick.bCanEverTick = true;
	
	ProjectileSpawner = CreateDefaultSubobject<UMCOProjectileSpawnComponent>(TEXT("NAME_ProjectileComponent"));
	PawnExtComponent = CreateDefaultSubobject<UMCOPawnExtensionComponent>(TEXT("NAME_PawnExtensionComponent"));
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
	
	PawnExtComponent->HandleControllerChanged();
	InitializeCharacter();
}

void AMCOCharacter::UnPossessed()
{
	Super::UnPossessed();
	
	PawnExtComponent->HandleControllerChanged();
}

void AMCOCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMCOCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void AMCOCharacter::StopCharacterFromMoving(bool bToStop)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(CharacterMC);
	
	CharacterMC->MovementMode = (true == bToStop) ? MOVE_None : MOVE_Walking;
}

void AMCOCharacter::StopCharacterFromTurning(bool bStopTuring)
{
	
}

void AMCOCharacter::ChangedMovementMode(const EMovementMode& InMode)
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	ISTRUE(nullptr != Movement);
	Movement->MovementMode = InMode;
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

	ISTRUE(ModeComponent);
	ModeComponent->GetCurrentAttachment()->TurnOffAllCollision();
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
	FVector Location;
	if (nullptr != GetMesh() && true == GetMesh()->DoesSocketExist(InSocketName))
	{
		Location = GetMesh()->GetSocketLocation(InSocketName);
	}
	return Location;
}

FTransform AMCOCharacter::GetSocketTransform(const FName& InSocketName)
{
	FTransform Transform;
	if (nullptr != GetMesh() && true == GetMesh()->DoesSocketExist(InSocketName))
	{
		Transform = GetMesh()->GetSocketTransform(InSocketName);
	}
	return Transform;
}

bool AMCOCharacter::IsCharacterOnGround() const
{
	ISTRUE_F(nullptr != GetCharacterMovement());
	return false == GetCharacterMovement()->IsFalling();
}

void AMCOCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeCharacter();
}

void AMCOCharacter::InitializeCharacter()
{
	// get ASC
	AMCOPlayerState* MCOPlayerState = GetPlayerState<AMCOPlayerState>();
	ensure(nullptr != MCOPlayerState);
	UMCOAbilitySystemComponent* ASC = MCOPlayerState->GetMCOAbilitySystemComponent();
	ensure(nullptr != ASC);

	// init ASC
	PawnExtComponent->InitializeAbilitySystem(ASC, MCOPlayerState);
	
	// give ability
	ISTRUE(GetLocalRole() == ROLE_Authority);
	ISTRUE(nullptr != CharacterData);
	ISTRUE(nullptr != CharacterData->AbilitySet);

	CharacterData->AbilitySet->GiveToAbilitySystem(ASC, &AbilitySetHandles, nullptr);
	
	// set delegates "after giving abilities" 
	MCOPlayerState->InitializeAbilitySystem();
	AttributeSet = ASC->GetSet<UMCOAttributeSet>();
	
	MCOPRINT(TEXT("[Init: %s] Health (%.1f/%.1f), Stemina (%.1f/%.1f), Stiffness (%.1f/%.1f)"),
		*CharacterName.ToString(),
		GetHealth(), GetMaxHealth(),
		GetStamina(), GetMaxStamina(),
		GetStiffness(), GetMaxStiffness()
	);

	// Bind tag event 
	MCOPlayerState->OnTagChangedDelegate.AddUObject(this, &ThisClass::OnTagChanged);
}

UAbilitySystemComponent* AMCOCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent ? PawnExtComponent->GetMCOAbilitySystemComponent() : nullptr;
}

UMCOAbilitySystemComponent* AMCOCharacter::GetMCOAbilitySystemComponent() const
{
	return PawnExtComponent ? PawnExtComponent->GetMCOAbilitySystemComponent() : nullptr;
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

bool AMCOCharacter::CheckCanBeDamaged(FGameplayTag InAttackTag)
{
	// except dodge...

	// removed and set effect tag
	// if (true == HasTag(FMCOCharacterTags::Get().InvincibleTag))
	// {
	// 	MCOLOG_C(MCOAbility, TEXT("Escaped from attack because I'm invincible"))
	// 	return false;
	// }
	if (InAttackTag == FMCOCharacterTags::Get().DragonAbility_Falldown)
	{
		return IsCharacterOnGround();
	}

	return true;
}

float AMCOCharacter::GetDamagedDegreeThenSetZero()
{
	const float Result = DamagedDegree;
	DamagedDegree = 0.0f;
	return Result;
}

void AMCOCharacter::SetDamagedDegree(const float& InDegree)
{
	DamagedDegree = InDegree;
}

float AMCOCharacter::GetCapsuleRadius() const
{
	const UCapsuleComponent* Capsule = GetCapsuleComponent();

	return (nullptr != Capsule) ? Capsule->GetScaledCapsuleRadius() : 0.0f;
}

void AMCOCharacter::SetIgnoreActors()
{
	IgnoreActors.Empty();
	IgnoreActors.Emplace(this);
	IgnoreActors.Append(ModeComponent->GetAttachments());
}

void AMCOCharacter::GetIgnoreActors(TArray<AActor*>& OutIgnoreActors) const
{
	OutIgnoreActors = IgnoreActors;
}

void AMCOCharacter::OnBeginCollision(const FCollisionBeginOverlapDelegate& InBeginDelegate, const FCollisionEndOverlapDelegate& InEndDelegate, const FName& InSocketName)
{
	AMCOAttachment* Attachment = ModeComponent->GetCurrentAttachment();
	ensure(Attachment);

	Attachment->OnBeginCollision(InBeginDelegate, InEndDelegate, InSocketName);
}

void AMCOCharacter::OnEndCollision(const FName& InSocketName)
{
	AMCOAttachment* Attachment = ModeComponent->GetCurrentAttachment();
	ensure(Attachment);

	Attachment->OnEndCollision(InSocketName);
}

AMCOProjectile* AMCOCharacter::SpawnProjectile(const TSubclassOf<AMCOProjectile>& InClass, const FName& InSocketName, const float& InSpeed, const float& InLifeSpan)
{
	FTransform SocketTransform = GetSocketTransform(InSocketName);
	SocketTransform.SetRotation(FQuat(FRotator(
		0.0f,
		SocketTransform.GetRotation().Rotator().Yaw,
		SocketTransform.GetRotation().Rotator().Roll
	)));
	
	return ProjectileSpawner->SpawnProjectile(InClass, this, SocketTransform, InSpeed, InLifeSpan);
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
	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();

	if (nullptr != ASC)
	{
		AbilitySetHandles.TakeFromAbilitySystem(ASC);
	}
	
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		DestroyAllAttachedActors();
		SetLifeSpan(0.1f);
	}

	SetActorHiddenInGame(true);
	PawnExtComponent->UninitializeAbilitySystem();
	
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