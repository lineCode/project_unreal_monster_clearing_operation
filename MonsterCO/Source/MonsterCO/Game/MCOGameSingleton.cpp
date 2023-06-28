#include "MCOGameSingleton.h"
#include "NiagaraEffect/MCONiagaraEffectData.h"


UMCOGameSingleton::UMCOGameSingleton()
{
	GETASSET(NiagaraEffectData, UMCONiagaraEffectData, TEXT("/Game/Data/Effect/DA_NiagaraEffectData.DA_NiagaraEffectData"));
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
