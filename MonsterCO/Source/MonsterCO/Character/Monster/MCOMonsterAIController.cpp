#include "MCOMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MCOAIKeys.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"



AMCOMonsterAIController::AMCOMonsterAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsPlayerState = true;

	GETASSET(BBAsset, UBlackboardData, TEXT("/Game/Monsters/Dragon/BB_Dragon.BB_Dragon"));
	GETASSET(BTAsset, UBehaviorTree, TEXT("/Game/Monsters/Dragon/BT_Dragon.BT_Dragon"));
}

void AMCOMonsterAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// InitPlayerState(); // removed because 2 PlayerState is made 
}

void AMCOMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void AMCOMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCOMonsterAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	ensure(true == UseBlackboard(BBAsset, BlackboardPtr));

	Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
	
	ensure(true == RunBehaviorTree(BTAsset));
}

void AMCOMonsterAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	ISTRUE(nullptr != BTComponent);
	BTComponent->StopTree();
}

void AMCOMonsterAIController::RestartAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	ISTRUE(nullptr != BTComponent);
	BTComponent->RestartTree();
}

UObject* AMCOMonsterAIController::GetTarget()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	ensure(true == UseBlackboard(BBAsset, BlackboardPtr));
	
	return BlackboardPtr->GetValueAsObject(BBKEY_TARGET);
}

void AMCOMonsterAIController::SetDamagedInBlackBoard(bool IsDamaged) const
{
	//MCOLOG_C(MCOMonsterAI, TEXT("Set damaged in black board : %s"), (true == IsDamaged) ? TEXT("DAMAGED") : TEXT("FINISHED"));
	
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	BlackboardPtr->SetValueAsBool(BBKEY_ISDAMAGED, IsDamaged);
}

