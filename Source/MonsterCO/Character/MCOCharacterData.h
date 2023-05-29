#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCOCharacterData.generated.h"

class UMCOAbilitySet;


UCLASS(BlueprintType)
class MONSTERCO_API UMCOCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UMCOCharacterData();

	UPROPERTY(EditAnywhere, Category = Info)
	FName CharacterName;

	// UPROPERTY(EditAnywhere, Category = Animation)
	// TSubclassOf<UAnimInstance> AnimInstance;
	//
	// UPROPERTY(EditAnywhere, Category = Mesh)
	// TObjectPtr<USkeletalMesh> Mesh;
	//
	// UPROPERTY(EditAnywhere, Category = Mesh)
	// FVector MeshRelativeLocation;
	//
	// UPROPERTY(EditAnywhere, Category = Mesh)
	// FRotator MeshRelativeRotation;

	// UPROPERTY(EditAnywhere, Category = Capsule)
	// float CapsuleHalfHeight;
	//
	// UPROPERTY(EditAnywhere, Category = Capsule)
	// float CapsuleRadius;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;
		
	UPROPERTY(EditAnywhere, Category = Ability)
	TObjectPtr<UMCOAbilitySet> AbilitySet;
};
