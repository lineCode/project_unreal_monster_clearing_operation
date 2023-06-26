#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCOMainWidgetData.generated.h"



class UMCOMainWidget;

UCLASS()
class MONSTERCO_API UMCOMainWidgetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EMCOWidgetState, TSubclassOf<UMCOMainWidget>> WidgetClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMCOMainWidget> HUDClass;
};
