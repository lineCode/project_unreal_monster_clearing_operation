#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "MCOAIKeys.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr ==  ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;

	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	PrevTargetRot = TargetRot;
	
	AIPawn->SetTurnVector(true, LookVector);
	return EBTNodeResult::InProgress;
}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	
	const FRotator InterpRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		PrevTargetRot,
		DeltaSeconds,
		AIPawn->GetAITurnSpeed()
	);

	PrevTargetRot += InterpRot - ControllingPawn->GetActorRotation();
	ControllingPawn->SetActorRotation(InterpRot);
	
	if (true == FMath::IsNearlyEqual(ControllingPawn->GetActorRotation().Yaw, TargetRot.Yaw, 5.0f))
	{
		AIPawn->SetTurnVector(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
