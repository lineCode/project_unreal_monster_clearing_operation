#include "MCOSoundManager.h"
#include "MCOSoundData.h"
#include "Components/AudioComponent.h"
#include "Game/MCOGameModeBase.h"
#include "Sound/SoundCue.h"



AMCOSoundManager::AMCOSoundManager()
{
	GETASSET(SoundData, UMCOSoundData, TEXT("/Game/Data/Sound/DA_GameStateSound.DA_GameStateSound"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NAME_AudioComponent"));
	SetRootComponent(AudioComponent);
}

void AMCOSoundManager::BeginPlay()
{
	Super::BeginPlay();

	// temp
	const AMCOGameModeBase* MCOGameMode = Cast<AMCOGameModeBase>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != MCOGameMode);
	const EMCOGameState CurrentState = MCOGameMode->GetGameState();
	PlaySoundByGameState(CurrentState);
}

void AMCOSoundManager::PlaySoundByGameState(const EMCOGameState& InState)
{
	ISTRUE(nullptr != SoundData->GetSoundCue(InState));
	
	PlaySound(SoundData->GetSoundCue(InState));
}

void AMCOSoundManager::PlaySound(USoundCue* SoundCue) const
{
	ISTRUE(nullptr != SoundCue);
	ISTRUE(nullptr != AudioComponent);

	AudioComponent->SetSound(SoundCue);
	AudioComponent->Activate();
}
