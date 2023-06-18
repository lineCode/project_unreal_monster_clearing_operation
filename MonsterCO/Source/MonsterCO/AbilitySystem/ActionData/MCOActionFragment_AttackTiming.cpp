#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"

float UMCOActionFragment_AttackTiming::GetDamageBeginTimeAfterPrevEndTime(uint8 InDamageIdx, float SpeedRate) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	float BeginTime = CalculateTime(DamageTimings[InDamageIdx].Begin, SpeedRate);

	if (InDamageIdx != 0)
	{
		BeginTime = CalculateTime(DamageTimings[InDamageIdx].Begin, SpeedRate) -
			CalculateTime(DamageTimings[InDamageIdx - 1].End, SpeedRate);
	}
	return BeginTime;
}

float UMCOActionFragment_AttackTiming::GetDamageExistTime(uint8 InDamageIdx, float SpeedRate) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));

	float Begin = GetDamageBeginTimeAfterPrevEndTime(InDamageIdx, SpeedRate);
	float End = CalculateTime(DamageTimings[InDamageIdx].End, SpeedRate);
	
	return End - Begin;
}

float UMCOActionFragment_AttackTiming::GetComboCheckTime(float SpeedRate) const
{
	return CalculateTime(NextComboFrameCount, SpeedRate);
}

UNiagaraSystem* UMCOActionFragment_AttackTiming::GetDamageNiagara(uint8 InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].DamagedNiagara;
}

float UMCOActionFragment_AttackTiming::GetStiffness(uint8 InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].Stiffness;
}

float UMCOActionFragment_AttackTiming::GetDamage(uint8 InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].Damage;
}

float UMCOActionFragment_AttackTiming::CalculateTime(float FrameCount, float SpeedRate) const
{
	return (FrameCount / MontageFrameRate) / SpeedRate;
}
