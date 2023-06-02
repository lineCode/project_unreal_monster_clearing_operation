#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOMontageDataSingle.generated.h"

class UMCOActionDefinition;

UCLASS()
class MONSTERCO_API UMCOMontageDataSingle : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FGameplayTag ActivationTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Definition, Instanced)
	TObjectPtr<UMCOActionDefinition> ActionDefinition;
	
public:
	UAnimMontage* GetMontage();
};
