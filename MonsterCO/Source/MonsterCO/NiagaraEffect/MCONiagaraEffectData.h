#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "MCONiagaraEffectData.generated.h"



class UNiagaraSystem;



USTRUCT(BlueprintType)
struct FMCONiagaraEffectByDamageAmount
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UNiagaraSystem>> EffectByDamageAmount = TArray<TObjectPtr<UNiagaraSystem>>();

public:
	UNiagaraSystem* GetNiagaraEffect(float InDamageAmount) const;	
};


USTRUCT(BlueprintType)
struct FMCODamagedEffect
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FMCONiagaraEffectByDamageAmount Instant;
	
	UPROPERTY(EditAnywhere)
	FMCONiagaraEffectByDamageAmount Duration;

public:
	UNiagaraSystem* GetNiagaraEffect(EMCOEffectPolicy InEffectPolicy, float InDamageAmount) const;	
};


UCLASS()
class MONSTERCO_API UMCONiagaraEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	// [ Attack effect ]
	// saved in data asset of abilities
	
	// [ Damaged effect ] 
	// 1. Attack type : Melee, Flame ... 
	// 2. Explosion or attack with duration
	// 3. Damage amount : -50.0f, -5.0f ... 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EMCONiagaraEffectType, FMCODamagedEffect> DamagedEffects;

public:
	UNiagaraSystem* GetDamagedNiagaraEffect(EMCONiagaraEffectType InEffectType, EMCOEffectPolicy InEffectPolicy, float InDamageAmount) const;	
};
