#include "MCOMonsterCharacter.h"
#include "MCOMonsterAIData.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MCOMonsterAIController.h"
#include "Character/MCOCharacterData.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "CharacterAttachment/MCOMonsterModeComponent.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


AMCOMonsterCharacter::AMCOMonsterCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GETASSET(CharacterData, UMCOCharacterData, TEXT("/Game/Data/Monster/Dragon/DA_Dragon.DA_Dragon"));
	GETASSET(MonsterAIData, UMCOMonsterAIData, TEXT("/Game/Data/Monster/Dragon/DA_DragonAIData.DA_DragonAIData"));

	bUseControllerRotationYaw = true;	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AIControllerClass = AMCOMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	ModeComponent = CreateDefaultSubobject<UMCOMonsterModeComponent>(TEXT("NAME_ModeComponent"));
	
	SetCharacterData();

	GetCharacterMovement()->MaxWalkSpeed = MonsterAIData->MoveSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = MonsterAIData->MoveSpeed;
	GetCharacterMovement()->MaxFlySpeed = MonsterAIData->FlySpeed;
}

void AMCOMonsterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	
}

void AMCOMonsterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
}

void AMCOMonsterCharacter::BeginPlay()
{
	Super::BeginPlay(); 
	
	ModeComponent->SetCurrentMode(static_cast<int32>(EMCOMonsterMode::Normal));
	ModeComponent->SpawnAttachment(this);

	// OnDamagedBegin.BindUObject(this, &ThisClass::StopAI);
	// OnDamagedEnd.BindUObject(this, &ThisClass::ContinueAI);
	
	// Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NAME_MonsterBody"));
	// Body->SetCollisionProfileName(CPROFILE_MCOCAPSULE);
	// Body->SetupAttachment(GetCapsuleComponent());
	// Body->SetHiddenInGame(false);
	// Body->SetCapsuleHalfHeight(220.0f);
	// Body->SetCapsuleRadius(100.0f);
	// Body->SetRelativeLocationAndRotation(FVector(-30.0f, 0.0f, -60.0f), FRotator(-90.0f, 180.0f, 180.0f));
	
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
}

void AMCOMonsterCharacter::StopCharacterFromMoving(bool bToStop)
{
	if (true == bToStop)
	{
		//StopAI();
	}
	else
	{
		//RestartAI();
	}
}

void AMCOMonsterCharacter::StopCharacterFromTurning(bool bStopTuring)
{
	
}

void AMCOMonsterCharacter::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::RESULT_LOSE)
	{
		//GetMCOAbilitySystemComponent()->RemoveEffectsOnResult();
		AbilitySetHandles.TakeFromAbilitySystem(AbilitySystemComponent.Get());
	}
}

void AMCOMonsterCharacter::InitializeCharacter()
{
	Super::InitializeCharacter();
	
	SetFlyMode(EMCOMonsterFlyMode::None);
	SetMovementMode(MOVE_Walking);
	SetGravity(1.0f);
	SetVelocity(FVector());

	RestartAI();
}

bool AMCOMonsterCharacter::CanActivateAbility(const FGameplayTag& InTag)
{
	ISTRUE_F(true == Super::CanActivateAbility(InTag));
	ISTRUE_F(nullptr != GetAbilitySystemComponent());

	if (InTag == FMCOCharacterTags::Get().JumpTag)
	{
		FGameplayTagContainer ActivationBlockedTags;
		ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
		ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
	
		return GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(ActivationBlockedTags) == false;
	}
	else if (InTag == FMCOCharacterTags::Get().AttackTag)
	{
		
	}

	return true;
}

bool AMCOMonsterCharacter::TryActivateAbilityByTag(const FGameplayTag& InTag) const
{
	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();
	ISTRUE_F(nullptr != ASC);

	return ASC->TryActivateAbilityByTag(InTag);
}

void AMCOMonsterCharacter::CancelAbilityByTag(const FGameplayTag& InTag) const
{
	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	return ASC->CancelAbilityByTag(InTag);
}

void AMCOMonsterCharacter::RestartAI() const
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->RestartAI();
}

void AMCOMonsterCharacter::StopAI() const
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->StopAI();
}

UObject* AMCOMonsterCharacter::GetTarget()
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ensure(AIController);	
	return AIController->GetTarget();
}

float AMCOMonsterCharacter::GetAIPatrolRadius() const
{
	return MonsterAIData->PatrolRadius;
}

float AMCOMonsterCharacter::GetAIDetectRange() const
{
	return MonsterAIData->DetectRange;
}

float AMCOMonsterCharacter::GetAIAttackRangeMin(const FGameplayTag& InTag) const
{
	return MonsterAIData->GetAbilityRangeMin(InTag) + GetCapsuleRadius();
}

float AMCOMonsterCharacter::GetAIAttackRangeMax(const FGameplayTag& InTag) const
{
	return MonsterAIData->GetAbilityRangeMax(InTag) + GetCapsuleRadius();
}

float AMCOMonsterCharacter::GetAITurnSpeed() const
{
	return MonsterAIData->TurnSpeed;
}

float AMCOMonsterCharacter::GetAIFlySpeed() const
{
	return MonsterAIData->FlySpeed;
}

float AMCOMonsterCharacter::GetHalfHeight() const
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();

	return (nullptr != Capsule) ? Capsule->GetScaledCapsuleHalfHeight() : 0.0f;
}

void AMCOMonsterCharacter::SetMovementMode(EMovementMode InMode)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(nullptr != CharacterMC);

	CharacterMC->MovementMode = InMode;
}

void AMCOMonsterCharacter::SetFlyMode(EMCOMonsterFlyMode InFlyMode)
{
	//MCOLOG_C(MCOMonsterAI, TEXT("SetFlyMode : %s"), *FHelper::GetEnumDisplayName(TEXT("EMCOMonsterFlyMode"), (int64)InFlyMode));
	
	FlyMode = InFlyMode;
}

EMCOMonsterFlyMode AMCOMonsterCharacter::GetFlyMode()
{
	return FlyMode;
}

void AMCOMonsterCharacter::SetGravity(float InGravity)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(nullptr != CharacterMC);

	CharacterMC->GravityScale = InGravity;
}

void AMCOMonsterCharacter::AddForce(FVector InForce)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(nullptr != CharacterMC);

	CharacterMC->AddImpulse(InForce, false);
}

FVector AMCOMonsterCharacter::GetVelocity()
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (nullptr == CharacterMC)
	{
		return FVector();
	}

	return CharacterMC->Velocity;
}

void AMCOMonsterCharacter::SetVelocity(FVector InVelocity)
{
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ISTRUE(nullptr != CharacterMC);

	CharacterMC->Velocity = InVelocity;
}

bool AMCOMonsterCharacter::IsTurning() const
{
	return false == TurnVector.IsNearlyZero();
}

FVector AMCOMonsterCharacter::GetTurnVector() const
{
	return TurnVector;
}

void AMCOMonsterCharacter::SetTurnVector(FVector InTurnVector)
{
	TurnVector = InTurnVector;
}

void AMCOMonsterCharacter::SetActivateActionDelegate(const FMCOActivateActionDelegate& InOnActivateAction)
{
	OnActivateActionDelegate = InOnActivateAction;
}

void AMCOMonsterCharacter::OnActivateAction()
{
	if (true == OnActivateActionDelegate.IsBound())
	{
		OnActivateActionDelegate.ExecuteIfBound();
	}
}

void AMCOMonsterCharacter::SetActionFinishedDelegate(const FMCOActionFinishedDelegate& InOnActionFinished)
{
	OnActionFinishedDelegate = InOnActionFinished;
}

void AMCOMonsterCharacter::OnActionFinished(EBTNodeResult::Type InResult)
{
	if (true == OnActionFinishedDelegate.IsBound())
	{
		OnActionFinishedDelegate.ExecuteIfBound(InResult);
	}
}

void AMCOMonsterCharacter::SetDamagedInBlackBoard(bool IsDamaged) const
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->SetDamagedInBlackBoard(IsDamaged);
}

void AMCOMonsterCharacter::Die()
{
	Super::Die();

	StopAI();
}
