#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCOMonstersData.generated.h"


class AMCOMonsterCharacter;



UCLASS()
class MONSTERCO_API UMCOMonstersData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Monster)
	TArray<TSubclassOf<AMCOMonsterCharacter>> Monsters;
};
