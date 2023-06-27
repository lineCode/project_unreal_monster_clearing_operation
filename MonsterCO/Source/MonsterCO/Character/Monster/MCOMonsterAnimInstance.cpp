#include "MCOMonsterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Character/MCOCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MCOMonsterAIInterface.h"


void UMCOMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ISTRUE(nullptr != Owner);
	ISTRUE(nullptr != MovementComponent);
	
	bIsFlying = MovementComponent->MovementMode == MOVE_Flying;


	
}

void UMCOMonsterAnimInstance::SetPawnSpeed(const FVector& InVelocity, const float& InDeltaSeconds)
{
	Super::SetPawnSpeed(InVelocity, InDeltaSeconds);
	
	ISTRUE(nullptr != MovementComponent);
	if (MovementComponent->MovementMode == MOVE_Flying)
	{
		IMCOMonsterAIInterface* MonsterAIInterface = Cast<IMCOMonsterAIInterface>(GetOwningActor());
		ISTRUE(nullptr != MonsterAIInterface);
		const float GoalVerticalSpeed = static_cast<float>(MonsterAIInterface->GetFlyMode()) - 1.0f;

		CurrentVerticalSpeed = FMath::FInterpTo(
			CurrentVerticalSpeed,
			GoalVerticalSpeed,
			InDeltaSeconds,
			10.0f
		);
	}
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
		//MCOLOG(TEXT("CurrentPawnDirection : %f"), CurrentPawnDirection);
	}
	else
	{
		const FVector TurnVector = MonsterAIInterface->GetTurnVector();
		CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(TurnVector, InActorRotation);
		//MCOLOG(TEXT("CurrentPawnDirection : %s -> %f"), *TurnVector.ToString(), CurrentPawnDirection);
	}

}
