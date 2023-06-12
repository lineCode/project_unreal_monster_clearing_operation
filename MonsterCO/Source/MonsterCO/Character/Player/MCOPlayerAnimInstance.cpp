#include "MCOPlayerAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Character/MCOCharacter.h"


void UMCOPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    PlayerInterface = Cast<IMCOPlayerInterface>(GetOwningActor());
}

void UMCOPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

    // --- Mode
    ISTRUE(nullptr != Owner);
    ISTRUE(nullptr != PlayerInterface);
    
    ModeType = PlayerInterface->GetModeType();
    bIsEquipped = PlayerInterface->IsEquipped();

    // --- Ability
    bIsDodging = Owner->HasTag(FMCOCharacterTags::Get().DodgeTag);
}

void UMCOPlayerAnimInstance::SetPawnSpeed(const FVector& InVelocity, const float& InDeltaSeconds)
{
    ISTRUE(nullptr != MovementComponent);
    
    const bool bIsAccelerate = MovementComponent->GetCurrentAcceleration().Length() > 0.0f;
    const float MaxSpeed = MovementComponent->GetMaxSpeed();

    CurrentPawnSpeed = FMath::FInterpTo(
        CurrentPawnSpeed,
        bIsAccelerate == true ? MaxSpeed : 0.0f,
        InDeltaSeconds,
        10.0f
    );
}

void UMCOPlayerAnimInstance::SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation)
{
    ISTRUE(nullptr != Owner);
    ISTRUE(nullptr != PlayerInterface);
    
    if (false == Owner->HasTag(FMCOCharacterTags::Get().DodgeTag)) // Stay the same direction on dodging
    {
        CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(PlayerInterface->GetInputWorldDirection(), InActorRotation);
    }
    if (true == Owner->HasTag(FMCOCharacterTags::Get().DashTag)) // Stay forward direction on dashing
    {
        CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(InVelocity, InActorRotation);
    }

}
