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

	FVector LookVector = (TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation()).GetSafeNormal();
	LookVector.Z = 0.0f;

	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	PrevTargetRot = TargetRot;
	
	if (true == FMath::IsNearlyEqual(ControllingPawn->GetActorRotation().Yaw, TargetRot.Yaw, 5.0f))
	{
		return EBTNodeResult::Succeeded;
	}

	//MCOLOG_C(MCOMonsterAI, TEXT("Set Turn vector : %s"), *LookVector.ToString());
	//MCOLOG_C(MCOMonsterAI, TEXT("Target Rot : %f -> %f"), ControllingPawn->GetActorRotation().Yaw, TargetRot.Yaw);
	
	AIPawn->SetTurnVector(LookVector);
	
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
		AIPawn->SetTurnVector(FVector(0.0f, 0.0f, 0.0f));
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
	
	//MCOLOG_C(MCOMonsterAI, TEXT("Turning Rot : %f -> %f"), InterpRot.Yaw, TargetRot.Yaw);

	
	if (true == FMath::IsNearlyEqual(ControllingPawn->GetActorRotation().Yaw, TargetRot.Yaw, 5.0f))
	{
		//MCOLOG_C(MCOMonsterAI, TEXT("Turn Succeeded"));
		AIPawn->SetTurnVector(FVector(0.0f, 0.0f, 0.0f));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
