#include "MCOSoundManager.h"
#include "MCOSoundData.h"
#include "Components/AudioComponent.h"
#include "Interface/MCOGameModeInterface.h"
#include "Sound/SoundCue.h"
#include "GameFramework/GameModeBase.h"



AMCOSoundManager::AMCOSoundManager()
{
	GETASSET(SoundData, UMCOSoundData, TEXT("/Game/Data/Sound/DA_GameStateSound.DA_GameStateSound"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NAME_AudioComponent"));
	SetRootComponent(AudioComponent);
}

void AMCOSoundManager::BeginPlay()
{
	Super::BeginPlay();

	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::PlaySoundByGameState);
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
