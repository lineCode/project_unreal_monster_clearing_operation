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
#include "CharacterAttachment/MCOMonsterAttachment.h"


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

void AMCOMonsterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMCOMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ModeComponent->SpawnAttachment(this);

	OnDamagedBegin.BindUObject(this, &ThisClass::StopAI);
	OnDamagedEnd.BindUObject(this, &ThisClass::ContinueAI);
	
	// Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NAME_MonsterBody"));
	// Body->SetCollisionProfileName(CPROFILE_MCOCAPSULE);
	// Body->SetupAttachment(GetCapsuleComponent());
	// Body->SetHiddenInGame(false);
	// Body->SetCapsuleHalfHeight(220.0f);
	// Body->SetCapsuleRadius(100.0f);
	// Body->SetRelativeLocationAndRotation(FVector(-30.0f, 0.0f, -60.0f), FRotator(-90.0f, 180.0f, 180.0f));
}

UObject* AMCOMonsterCharacter::GetTarget()
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ensure(AIController);	
	return AIController->GetTarget();
}

float AMCOMonsterCharacter::GetAIPatrolRadius()
{
	return MonsterAIData->PatrolRadius;
}

float AMCOMonsterCharacter::GetAIDetectRange()
{
	return MonsterAIData->DetectRange;
}

float AMCOMonsterCharacter::GetAIMeleeRange()
{
	return MonsterAIData->AttackRange_Melee + GetCapsuleRadius(); // Diff from attack data
}

float AMCOMonsterCharacter::GetAITurnSpeed()
{
	return MonsterAIData->TurnSpeed;
}

FVector AMCOMonsterCharacter::GetAITurnVector()
{
	return TurnVector;
}

bool AMCOMonsterCharacter::IsTurning()
{
	return bIsTurning;
}

void AMCOMonsterCharacter::SetAIAttackDelegate(const FAICharacterAITaskFinishedDelegate& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

bool AMCOMonsterCharacter::CanAttack()
{
	ISTRUE_F(nullptr != GetAbilitySystemComponent());
	
	FGameplayTagContainer ActivationBlockedTags;
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().AttackTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
	
	return GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(ActivationBlockedTags) == false;
}

void AMCOMonsterCharacter::SetTurnVector(const bool InIsTurning, const FVector& InTurnVector)
{
	bIsTurning = InIsTurning;
	TurnVector = InTurnVector;
}

void AMCOMonsterCharacter::AttackByAI()
{
	UMCOAbilitySystemComponent* ASC = GetMCOAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	ASC->TryActivateAbilityByTag(FMCOCharacterTags::Get().AttackTag);
}

void AMCOMonsterCharacter::ContinueAI()
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->ContinueAI();
}

void AMCOMonsterCharacter::StopAI()
{
	AMCOMonsterAIController* AIController = Cast<AMCOMonsterAIController>(GetController());
	ISTRUE(nullptr != AIController);
	AIController->StopAI();
}

void AMCOMonsterCharacter::OffAllCollision()
{
	Super::OffAllCollision();

	GetModeComponent()->GetAttachment()->TurnOffAllCollision();
}
