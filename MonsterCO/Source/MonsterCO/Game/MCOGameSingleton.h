#pragma once

#include "MonsterCO.h"
#include "MCOGameSingleton.generated.h"


class USoundCue;
class UMCOSoundData;

UCLASS()
class MONSTERCO_API UMCOGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMCOGameSingleton();
	static UMCOGameSingleton& Get();

	void PlaySoundByGameState(const UWorld* InWorld, const EMCOGameState& InState);	

protected:
	void PlaySound(const UWorld* InWorld, USoundCue* SoundCue) const;

	UPROPERTY()
	TObjectPtr<UMCOSoundData> SoundData;
};
