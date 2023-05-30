#include "MCOMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MCOAIKeys.h"


AMCOMonsterAIController::AMCOMonsterAIController(const FObjectInitializer& ObjectInitializer)
	 :Super(ObjectInitializer)
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

void AMCOMonsterAIController::ContinueAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	ISTRUE(nullptr != BTComponent);
	BTComponent->RestartTree();
}

