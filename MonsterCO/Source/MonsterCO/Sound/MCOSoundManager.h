#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "MCOSoundManager.generated.h"


class USoundCue;
class UAudioComponent;
class UMCOSoundData;



UCLASS()
class MONSTERCO_API AMCOSoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOSoundManager();

	virtual void BeginPlay() override;
	
public:
	void PlaySoundByGameState(const EMCOGameState& InState);	

protected:
	void PlaySound(USoundCue* SoundCue) const;

protected:
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UPROPERTY()
	TObjectPtr<UMCOSoundData> SoundData;

};
