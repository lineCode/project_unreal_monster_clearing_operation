#pragma once

#include "MonsterCO.h"
#include "MCOGameSingleton.generated.h"


class UMCONiagaraEffectData;

UCLASS()
class MONSTERCO_API UMCOGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMCOGameSingleton();
	static UMCOGameSingleton& Get();
	
public:
	UPROPERTY()
	TObjectPtr<UMCONiagaraEffectData> NiagaraEffectData;
};
