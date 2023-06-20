#include "BTService_Detect.h"
#include "MCOAIKeys.h"
#include "Physics/MCOPhysics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr ==  ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Detect),
		false,
		ControllingPawn
	);

	bool HitDetected = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CHANNEL_ACTION_TRACE,
		FCollisionShape::MakeSphere(DetectRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(World, Center, DetectRadius, 16, (HitDetected == true) ? FColor::Blue : FColor::Yellow, false, 0.2f);
#endif

	if (true == HitDetected)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			
			if (nullptr != Pawn && true == Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

#if ENABLE_DRAW_DEBUG
				// DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				// DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
#endif
				return;
			}
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
}
