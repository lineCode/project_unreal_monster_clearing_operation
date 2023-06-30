#include "AI/BTTask_FlyUp.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTTask_FlyUp::UBTTask_FlyUp()
{
	NodeName = TEXT("Fly Up");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FlyUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr ==  ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (AIPawn->GetFlyMode() != EMCOMonsterFlyMode::None)
	{
		return EBTNodeResult::Failed;
	}
	
	const FVector PawnLocation = ControllingPawn->GetActorLocation();
	FlyGoal = PawnLocation + FVector(0.0f, 0.0f, FlyDistance);

	// AIPawn->SetMovementMode(MOVE_Flying);
	AIPawn->SetFlyMode(EMCOMonsterFlyMode::Up);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_FlyUp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr ==  ControllingPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AIPawn->AddForce(FVector(0.0f, 0.0f, FlyForce));

	if (FlyGoal.Z < ControllingPawn->GetActorLocation().Z) 
	{
		// AIPawn->SetGravity(0.0f);
		// AIPawn->SetVelocity(FVector());
		AIPawn->SetFlyMode(EMCOMonsterFlyMode::Hold);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
