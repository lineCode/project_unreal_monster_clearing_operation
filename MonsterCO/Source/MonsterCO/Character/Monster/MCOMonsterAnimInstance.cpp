#include "MCOMonsterAnimInstance.h"
#include "KismetAnimationLibrary.h"


void UMCOMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	
}

void UMCOMonsterAnimInstance::SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation)
{
	ISTRUE(nullptr != Owner);

	IMCOMonsterAIInterface* MonsterAIInterface = Cast<IMCOMonsterAIInterface>(GetOwningActor());
	ISTRUE(nullptr != MonsterAIInterface);
	
	// Turn in place
	if (false == MonsterAIInterface->IsTurning())
	{
		CurrentPawnDirection = 0.0f;
	}
	else
	{
		const FVector TurnVector = MonsterAIInterface->GetAITurnVector();
		CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(TurnVector, InActorRotation);
	}

}
