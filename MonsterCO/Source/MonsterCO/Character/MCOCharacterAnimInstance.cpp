#include "Character/MCOCharacterAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MCOCharacter.h"


UMCOCharacterAnimInstance::UMCOCharacterAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UMCOCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Owner = Cast<AMCOCharacter>(GetOwningActor());
	ISTRUE(Owner != nullptr);
	MovementComponent = Owner->GetCharacterMovement();
}

void UMCOCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ISTRUE(nullptr != Owner);
	ISTRUE(nullptr != MovementComponent);

	SetPawnSpeed(MovementComponent->Velocity, DeltaSeconds);
	
	Velocity = MovementComponent->Velocity;
	
	bIsIdle = CurrentPawnSpeed < MovingThreshould;
	bIsFalling = MovementComponent->IsFalling();
	bIsJumping = bIsFalling & (JumpingThreshould < Velocity.Z);

	SetPawnDirection(Velocity, Owner->GetActorRotation());
}

void UMCOCharacterAnimInstance::SetPawnSpeed(const FVector& InVelocity, const float& InDeltaSeconds)
{
	CurrentPawnSpeed = InVelocity.Size2D();
}

void UMCOCharacterAnimInstance::SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation)
{
	CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(InVelocity, InActorRotation);
}
