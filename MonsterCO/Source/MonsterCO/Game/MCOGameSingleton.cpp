#include "MCOGameSingleton.h"
#include "Sound/MCOSoundData.h"
#include "Sound/SoundCue.h"



UMCOGameSingleton::UMCOGameSingleton()
{
	GETASSET(SoundData, UMCOSoundData, TEXT("/Game/Data/Sound/DA_GameStateSound.DA_GameStateSound"));
}

UMCOGameSingleton& UMCOGameSingleton::Get()
{
	UMCOGameSingleton* Singleton = CastChecked<UMCOGameSingleton>(GEngine->GameSingleton);

	if (nullptr != Singleton)
	{
		return *Singleton;
	}

	MCOLOG(TEXT("Invalid Game Singleton"));
	return *NewObject<UMCOGameSingleton>();
}

void UMCOGameSingleton::PlaySoundByGameState(const UWorld* InWorld, const EMCOGameState& InState)
{
	ISTRUE(nullptr != SoundData->GetSoundCue(InState));
	ISTRUE(nullptr != InWorld);
	
	PlaySound(InWorld, SoundData->GetSoundCue(InState));
}

void UMCOGameSingleton::PlaySound(const UWorld* InWorld, USoundCue* SoundCue) const
{
	ISTRUE(nullptr != SoundCue);
	ISTRUE(nullptr != InWorld);

	UGameplayStatics::PlaySound2D(InWorld, SoundCue);
}
