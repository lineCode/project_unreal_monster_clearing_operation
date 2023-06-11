#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCOSoundData.generated.h"

class USoundCue;

UCLASS()
class MONSTERCO_API UMCOSoundData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USoundCue* GetSoundCue(const EMCOGameState& InState);
	
protected:
	UPROPERTY(EditAnywhere, Category = Sound)
	TMap<EMCOGameState, TObjectPtr<USoundCue>> GameStateSound;
};
