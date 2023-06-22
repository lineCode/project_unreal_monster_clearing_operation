#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "MCOActionFragment_MonsterAI.generated.h"

UCLASS()
class MONSTERCO_API UMCOActionFragment_MonsterAI : public UMCOActionFragment
{
	GENERATED_BODY()
	
public:
	void OnActionFinished(const AActor* InCurrentActor, const bool& InSucceeded) const;
};