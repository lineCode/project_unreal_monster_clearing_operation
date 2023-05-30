#pragma once

#include "MonsterCO.h"
#include "Animation/AnimInstance.h"
#include "MCOPlayerAnimInstance.generated.h"

class AMCOPlayerCharacter;
class UCharacterMovementComponent;


UCLASS()
class MONSTERCO_API UMCOPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMCOPlayerAnimInstance();

private:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<AMCOPlayerCharacter> Owner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	EMCOModeType ModeType; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsIdle : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsFalling:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsDodging:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsEquipped:1;
};
