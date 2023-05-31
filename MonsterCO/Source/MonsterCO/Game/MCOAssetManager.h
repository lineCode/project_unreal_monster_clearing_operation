#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MCOAssetManager.generated.h"

UCLASS()
class MONSTERCO_API UMCOAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	virtual void StartInitialLoading() override;
};
