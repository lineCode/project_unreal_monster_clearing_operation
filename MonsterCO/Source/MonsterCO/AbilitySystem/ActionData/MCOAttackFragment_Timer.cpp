#include "AbilitySystem/ActionData/MCOAttackFragment_Timer.h"

float UMCOAttackFragment_Timer::GetDamageBeginTimeAfterPrevEndTime(uint8 InDamageIdx, float SpeedRate)
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

float UMCOAttackFragment_Timer::GetDamageExistTime(uint8 InDamageIdx, float SpeedRate)
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));

	float Begin = GetDamageBeginTimeAfterPrevEndTime(InDamageIdx, SpeedRate);
	float End = CalculateTime(DamageTimings[InDamageIdx].End, SpeedRate);
	
	return End - Begin;
}

float UMCOAttackFragment_Timer::GetComboCheckTime(float SpeedRate)
{
	return CalculateTime(NextComboFrameCount, SpeedRate);
}

float UMCOAttackFragment_Timer::CalculateTime(float FrameCount, float SpeedRate)
{
	return (FrameCount / MontageFrameRate) / SpeedRate;
}
