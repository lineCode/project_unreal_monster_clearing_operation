#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOActionData.generated.h"

class UMCOActionDefinition;

UCLASS()
class MONSTERCO_API UMCOActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FGameplayTag ActivationTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Definition, Instanced)
	TObjectPtr<UMCOActionDefinition> ActionDefinition;
	
};
