#include "BTService_Detect.h"
#include "MCOAIKeys.h"
#include "Physics/MCOPhysics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MCOMonsterAIInterface.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect player");
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

	const FVector Center = ControllingPawn->GetActorLocation();
	const UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	const float DetectRadius = AIPawn->GetAIDetectRange();
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Detect),
		false,
		ControllingPawn
	);

	const bool HitDetected = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CHANNEL_ACTION_TRACE,
		FCollisionShape::MakeSphere(DetectRadius),
		Params
	);

#if ENABLE_DRAW_DEBUG
	//DrawDebugSphere(World, Center, DetectRadius, 16, (HitDetected == true) ? FColor::Blue : FColor::Yellow, false, 0.2f);
#endif

	if (true == HitDetected)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Target = Cast<APawn>(OverlapResult.GetActor());
			
			if (nullptr != Target && true == Target->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Target);

				// use "Last Target Pose" to abort attack tasks...
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_LASTTARGETPOS, Target->GetActorLocation());

#if ENABLE_DRAW_DEBUG
				// DrawDebugPoint(World, Target->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				// DrawDebugLine(World, ControllingPawn->GetActorLocation(), Target->GetActorLocation(), FColor::Green, false, 0.27f);
#endif
				return;
			}
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_LASTTARGETPOS, nullptr);
}
