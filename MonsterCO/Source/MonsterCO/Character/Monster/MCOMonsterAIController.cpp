#include "MCOMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MCOAIKeys.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"



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
	
	StopAI();
}

void AMCOMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddDynamic(this, &ThisClass::OnChangeGameState);
}

void AMCOMonsterAIController::OnChangeGameState(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::FIGHT)
	{
		RunAI();
	}
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

UObject* AMCOMonsterAIController::GetTarget()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	ensure(true == UseBlackboard(BBAsset, BlackboardPtr));
	
	return BlackboardPtr->GetValueAsObject(BBKEY_TARGET);
}

