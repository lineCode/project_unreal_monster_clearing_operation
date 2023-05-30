#include "MCOPlayerAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MCOPlayerCharacter.h"
#include "PlayerWeapon/MCOPlayerModeComponent.h"
#include "Ability_Character/MCOCharacterTags.h"


UMCOPlayerAnimInstance::UMCOPlayerAnimInstance()
{
    MovingThreshould = 3.0f;
    JumpingThreshould = 100.0f;
}

void UMCOPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Owner = Cast<AMCOPlayerCharacter>(GetOwningActor());
    ISTRUE(Owner != nullptr);
    MovementComponent = Owner->GetCharacterMovement();
}

void UMCOPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
    
    ISTRUE(Owner != nullptr);
    ISTRUE(MovementComponent != nullptr);
    
    bool bIsAccelerate = MovementComponent->GetCurrentAcceleration().Length() > 0.0f;
    float MaxSpeed = MovementComponent->GetMaxSpeed();

    CurrentPawnSpeed = FMath::FInterpTo(
        CurrentPawnSpeed,
        bIsAccelerate == true ? MaxSpeed : 0.0f,
        DeltaSeconds,
        10.0f
    );

    Velocity = MovementComponent->Velocity;
    bIsIdle = CurrentPawnSpeed < MovingThreshould;
    bIsFalling = MovementComponent->IsFalling();
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
    
    ModeType = Owner->GetModeType();
    bIsEquipped = Owner->GetModeComponent()->IsEquipped();

    // --- Ability
    ISTRUE(Owner->GetAbilitySystemComponent() != nullptr);
    
    bIsDodging = Owner->HasTag(FMCOCharacterTags::Get().DodgeTag);
    if (false == bIsDodging)
    {
        CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(Owner->GetInputDirection(), Owner->GetActorRotation());
    }
    if (true == Owner->HasTag(FMCOCharacterTags::Get().DashTag))
    {
        CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Owner->GetActorRotation());
    }
}
