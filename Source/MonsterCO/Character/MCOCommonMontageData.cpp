#include "MCOCommonMontageData.h"

UMCOCommonMontageData::UMCOCommonMontageData()
{
	FrameRate = 30.0f;
	MontageSectionName = NAME_None;
}

float UMCOCommonMontageData::GetDamageBeginFrameCountAfterEndTime(uint8 CurrentIdx, float AttackSpeedRate)
{
	ensure(DamageTimings.IsValidIndex(CurrentIdx));
	float BeginTime = GetFrameCountByConsideringSpeed(DamageTimings[CurrentIdx].DamageBeginFrameCount, AttackSpeedRate);

	if (CurrentIdx != 0)
	{
		BeginTime = GetFrameCountByConsideringSpeed(DamageTimings[CurrentIdx].DamageBeginFrameCount, AttackSpeedRate) -
			GetFrameCountByConsideringSpeed(DamageTimings[CurrentIdx - 1].DamageEndFrameCount, AttackSpeedRate);
	}
	return BeginTime;
}

float UMCOCommonMontageData::GetDamageExistFrameCount(uint8 CurrentIdx, float AttackSpeedRate)
{
	ensure(DamageTimings.IsValidIndex(CurrentIdx));

	float Begin = GetDamageBeginFrameCountAfterEndTime(CurrentIdx, AttackSpeedRate);
	float End = GetFrameCountByConsideringSpeed(DamageTimings[CurrentIdx].DamageEndFrameCount, AttackSpeedRate);
	
	return End - Begin;
}

float UMCOCommonMontageData::GetFrameCountByConsideringSpeed(float FrameCount, float AttackSpeedRate)
{
	return (FrameCount / FrameRate) / AttackSpeedRate;
}
