#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCOMonsterAIData.generated.h"

UCLASS()
class MONSTERCO_API UMCOMonsterAIData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UMCOMonsterAIData();

	UPROPERTY(EditAnywhere, Category = AI)
	float PatrolRadius;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float DetectRange;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float AttackRange_Melee;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float AttackRange_Wide;
	
	UPROPERTY(EditAnywhere, Category = AI)
	float TurnSpeed;
};
