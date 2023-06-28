#include "AI/BTTask_FlyDown.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MCOMonsterAIInterface.h"



UBTTask_FlyDown::UBTTask_FlyDown()
{
	NodeName = TEXT("Fly Down");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FlyDown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (AIPawn->GetFlyMode() == EMCOMonsterFlyMode::None)
	{
		return EBTNodeResult::Succeeded;
	}
	
	AIPawn->SetFlyMode(EMCOMonsterFlyMode::Down);
		
	return EBTNodeResult::InProgress;
}

void UBTTask_FlyDown::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	
	UCharacterMovementComponent* CharacterMC = Cast<UCharacterMovementComponent>(ControllingPawn->GetMovementComponent());
	if (nullptr == CharacterMC)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	FFindFloorResult OutResult;
	CharacterMC->ComputeFloorDist(ControllingPawn->GetActorLocation(), 4000.0f, 8000.0f, OutResult, 1.0f);
	if (OutResult.FloorDist < CharacterMC->Velocity.Length() * 0.2f)
	{
		AIPawn->SetMovementMode(MOVE_Falling);
		AIPawn->SetGravity(1.0f);
		AIPawn->SetFlyMode(EMCOMonsterFlyMode::None);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
