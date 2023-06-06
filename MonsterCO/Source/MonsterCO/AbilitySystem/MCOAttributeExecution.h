#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "MCOAttributeExecution.generated.h"

UCLASS()
class MONSTERCO_API UMCOAttributeExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
		
public:
	UMCOAttributeExecution();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};
