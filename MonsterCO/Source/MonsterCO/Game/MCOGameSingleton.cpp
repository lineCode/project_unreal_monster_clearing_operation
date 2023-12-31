#include "MCOGameSingleton.h"

UMCOGameSingleton::UMCOGameSingleton()
{
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
