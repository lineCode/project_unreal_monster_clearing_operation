#pragma once

#include "MonsterCO.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Collision.generated.h"

UCLASS()
class MONSTERCO_API UMCOActionFragment_Collision : public UMCOActionFragment
{
	GENERATED_BODY()

public:
	FVector GetAttackDirection(const AActor* InActor) const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMCOCharacterDirection AttackDirection = EMCOCharacterDirection::Front;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// FVector AdditiveLocationFromFront = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackLength = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRadius = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketName = NAME_None;
};
