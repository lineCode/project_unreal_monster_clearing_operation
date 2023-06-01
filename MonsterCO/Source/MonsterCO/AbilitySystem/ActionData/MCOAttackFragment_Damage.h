#pragma once

#include "CoreMinimal.h"
#include "MCOAttackDefinition.h"
#include "MCOAttackFragment_Damage.generated.h"


UCLASS()
class MONSTERCO_API UMCOAttackFragment_Damage : public UMCOAttackFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Stiffness = 0.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 0.0f;
};
