#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCOPlayerControlData.generated.h"


UCLASS(BlueprintType)
class MONSTERCO_API UMCOPlayerControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float JumpZVelocity;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float GravityScale;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	TMap<EMCOCharacterSpeed, float> WalkSpeeds;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float BaseLookRate;
	
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FVector SpringArmRelativeLocation;
	
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator SpringArmRelativeRotation;
	
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
