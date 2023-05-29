#pragma once

#include "MonsterCO.h"
#include "GameplayEffectExecutionCalculation.h"
#include "MCODamageExecution.generated.h"

UCLASS()
class MONSTERCO_API UMCODamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UMCODamageExecution();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
