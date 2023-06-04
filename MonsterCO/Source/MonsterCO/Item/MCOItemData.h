#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCOItemData.generated.h"



UCLASS()
class MONSTERCO_API UMCOItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EMCOItemType Type;
	
};
