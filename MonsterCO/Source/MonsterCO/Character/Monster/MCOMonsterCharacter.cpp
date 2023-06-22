#include "MCOMonsterCharacter.h"
#include "MCOMonsterAIData.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
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

	GetMesh()->SetGenerateOverlapEvents(true);
	
	bUseControllerRotationYaw = true;	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AIControllerClass = AMCOMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	ModeComponent = CreateDefaultSubobject<UMCOMonsterModeComponent>(TEXT("NAME_ModeComponent"));

	bIsTurning = false;
	
	SetCharacterData();
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
		StopAI();
	}
	else
	{
		ContinueAI();
	}
}

void AMCOMonsterCharacter::StopCharacterFromTurning(bool bStopTuring)
{
	
}

void AMCOMonsterCharacter::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::RESULT_LOSE)
	{
		RemoveEffectsOnResult();
	}
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
	else if (InTag == FMCOCharacterTags::Get().DragonAbility_Claw)
	{
		
	}
	else if (InTag == FMCOCharacterTags::Get().DragonAbility_Fireball)
	{
		
	}
	else if (InTag == FMCOCharacterTags::Get().DragonAbility_Breath)
	{
		
	}

	return true;
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

FVector AMCOMonsterCharacter::GetAITurnVector() const
{
	return TurnVector;
}

bool AMCOMonsterCharacter::IsTurning() const
{
	return bIsTurning;
}

void AMCOMonsterCharacter::SetActionDelegate(const FMCOAICharacterTaskFinishedDelegate& InOnAttackFinished)
{
	OnAttackFinishedDelegate = InOnAttackFinished;
}

void AMCOMonsterCharacter::OnActionFinished(const EBTNodeResult::Type& InResult) const
{
	OnAttackFinishedDelegate.ExecuteIfBound(InResult);
}

void AMCOMonsterCharacter::SetTurnVector(const bool InIsTurning, const FVector& InTurnVector)
{
	bIsTurning = InIsTurning;
	TurnVector = InTurnVector;
}

void AMCOMonsterCharacter::Attack(const FGameplayTag& InTag) const
{
	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	ASC->TryActivateAbilityByTag(InTag);
}

void AMCOMonsterCharacter::ContinueAI() const
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->ContinueAI();
}

void AMCOMonsterCharacter::StopAI() const
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->StopAI();
}
