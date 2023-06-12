#include "AbilitySystem/ActionData/MCOActionFragment_Timer.h"

float UMCOActionFragment_Timer::GetDamageBeginTimeAfterPrevEndTime(uint8 InDamageIdx, float SpeedRate) const
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

float UMCOActionFragment_Timer::GetDamageExistTime(uint8 InDamageIdx, float SpeedRate) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));

	float Begin = GetDamageBeginTimeAfterPrevEndTime(InDamageIdx, SpeedRate);
	float End = CalculateTime(DamageTimings[InDamageIdx].End, SpeedRate);
	
	return End - Begin;
}

float UMCOActionFragment_Timer::GetComboCheckTime(float SpeedRate) const
{
	return CalculateTime(NextComboFrameCount, SpeedRate);
}

UNiagaraSystem* UMCOActionFragment_Timer::GetDamageNiagara(uint8 InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].DamagedNiagara;
}

float UMCOActionFragment_Timer::CalculateTime(float FrameCount, float SpeedRate) const
{
	return (FrameCount / MontageFrameRate) / SpeedRate;
}
