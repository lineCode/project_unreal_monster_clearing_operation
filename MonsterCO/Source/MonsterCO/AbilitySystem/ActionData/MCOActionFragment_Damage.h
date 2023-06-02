#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Damage.generated.h"


UCLASS()
class MONSTERCO_API UMCOActionFragment_Damage : public UMCOActionFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Stiffness = 0.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 0.0f;
};
