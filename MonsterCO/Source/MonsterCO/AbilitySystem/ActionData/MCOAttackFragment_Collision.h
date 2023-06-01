#pragma once

#include "MonsterCO.h"
#include "MCOAttackDefinition.h"
#include "MCOAttackFragment_Collision.generated.h"

UCLASS()
class MONSTERCO_API UMCOAttackFragment_Collision : public UMCOAttackFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMCOCharacterDirection AttackDirection = EMCOCharacterDirection::Front;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AdditiveLocationFromFront = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackLength = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRadius = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketName = NAME_None;
};
