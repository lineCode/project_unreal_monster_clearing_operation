#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCOItemData.generated.h"


UCLASS()
class MONSTERCO_API UMCOItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(ITEMDATA_NAME, GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EMCOItemType Type;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UMaterialInstance> Material;
};
