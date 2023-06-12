#pragma once

#include "MonsterCO.h"
#include "Animation/AnimInstance.h"
#include "MCOCharacterAnimInstance.generated.h"


class AMCOCharacter;
class UCharacterMovementComponent;



UCLASS()
class MONSTERCO_API UMCOCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMCOCharacterAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void SetPawnSpeed(const FVector& InVelocity, const float& InDeltaSeconds);
	virtual void SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<AMCOCharacter> Owner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsIdle : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsFalling:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	
};
