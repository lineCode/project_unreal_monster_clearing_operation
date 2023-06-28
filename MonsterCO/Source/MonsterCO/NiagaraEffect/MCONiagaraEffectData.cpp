#include "NiagaraEffect/MCONiagaraEffectData.h"



UNiagaraSystem* FMCONiagaraEffectByDamageAmount::GetNiagaraEffect(float InDamageAmount) const
{
	InDamageAmount = FMath::Abs(InDamageAmount);
	
	const int32 CountMax = EffectByDamageAmount.Num(); 
	const float DamageGap = LARGE_DAMAGE_AMOUNT / CountMax; 
	
	for (int32 i = CountMax - 1; i >= 0; i--)
	{
		const float CurDamage = DamageGap * i;

		if (CurDamage <= InDamageAmount && nullptr != EffectByDamageAmount[i])
		{
			return EffectByDamageAmount[i];
		}
	}
	
	return nullptr;
}

UNiagaraSystem* FMCODamagedEffect::GetNiagaraEffect(EMCOEffectPolicy InEffectPolicy, float InDamageAmount) const
{
	if (InEffectPolicy == EMCOEffectPolicy::Instant)
	{
		return Instant.GetNiagaraEffect(InDamageAmount);
	}
	else
	{
		return Duration.GetNiagaraEffect(InDamageAmount);
	}
}

UNiagaraSystem* UMCONiagaraEffectData::GetDamagedNiagaraEffect(EMCONiagaraEffectType InEffectType, EMCOEffectPolicy InEffectPolicy, float InDamageAmount) const
{
	ISTRUE_N(true == DamagedEffects.Contains(InEffectType));
	
	return DamagedEffects[InEffectType].GetNiagaraEffect(InEffectPolicy, InDamageAmount);
}
