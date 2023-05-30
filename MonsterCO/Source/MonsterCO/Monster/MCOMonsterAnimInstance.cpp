#include "MCOMonsterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Ability_Character/MCOCharacterTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "Character/MCOCharacter.h"

UMCOMonsterAnimInstance::UMCOMonsterAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

bool UMCOMonsterAnimInstance::IsTurnDone()
{
	return false;
}

void UMCOMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AMCOCharacter>(GetOwningActor());
	ISTRUE(Owner != nullptr);
	MovementComponent = Owner->GetCharacterMovement();
}

void UMCOMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ISTRUE(nullptr != MovementComponent);
		
	Velocity = MovementComponent->Velocity;
	CurrentPawnSpeed = Velocity.Size2D();
	
	bIsIdle = CurrentPawnSpeed < MovingThreshould;
	bIsFalling = MovementComponent->IsFalling();
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	
	CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Owner->GetActorRotation());

	// Turn in place
	IMCOMonsterAIInterface* AIPawn = Cast<IMCOMonsterAIInterface>(Owner);
	ISTRUE(nullptr != AIPawn);
	if (false == AIPawn->IsTurning())
	{
		CurrentPawnDirection = 0.0f;
	}
	else
	{
		const FVector TurnVector = AIPawn->GetAITurnVector();
		CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(TurnVector, Owner->GetActorRotation());
	}
}
