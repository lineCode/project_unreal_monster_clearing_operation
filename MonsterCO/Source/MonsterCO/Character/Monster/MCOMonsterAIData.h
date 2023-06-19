#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOMonsterAIData.generated.h"


USTRUCT()
struct FMCOAIRange
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Start = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float End = 0.0f;
};


UCLASS()
class MONSTERCO_API UMCOMonsterAIData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = AI)
	float PatrolRadius;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float DetectRange;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float TurnSpeed;

protected:	
	UPROPERTY(EditAnywhere, Category = AI)
	TMap<FGameplayTag, FMCOAIRange> AbilityRanges;

public:
	float GetAbilityRangeMax(const FGameplayTag& InTag);
	float GetAbilityRangeMin(const FGameplayTag& InTag);
};
