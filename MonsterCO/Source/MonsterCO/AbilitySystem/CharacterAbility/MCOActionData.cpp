#include "MCOActionData.h"


UAnimMontage* UMCOActionData::GetMontage(uint8 InComboIdx)
{
	ensure(InComboIdx < AttackActionDatas.Num());
	return AttackActionDatas[InComboIdx].MontageToPlay;
}

float UMCOActionData::GetFrameRate(uint8 InComboIdx)
{
	ensure(InComboIdx < AttackActionDatas.Num());
	return AttackActionDatas[InComboIdx].MontageFrameRate;
}

const FMCOAttackAttributes UMCOActionData::GetAttackAttributes(uint8 InComboIdx)
{
	ensure(InComboIdx < AttackActionDatas.Num());
	return AttackActionDatas[InComboIdx].AttackAttributes;
}

const FMCOAttackCollisionData UMCOActionData::GetCollisionData(uint8 InComboIdx)
{
	ensure(InComboIdx < AttackActionDatas.Num());
	return AttackActionDatas[InComboIdx].AttackCollisionData;
}

const FMCODamageFrameCounts UMCOActionData::GetDamageTimingFrameCount(uint8 InDamageIdx, uint8 InComboIdx)
{
	ensure(InComboIdx < AttackActionDatas.Num());
	return AttackActionDatas[InComboIdx].AttackDamageTimings[InDamageIdx];
}





float UMCOActionData::GetDamageBeginFrameCountAfterEndTime(uint8 InDamageIdx, uint8 InComboIdx, float AttackSpeedRate)
{
	float Begin = GetDamageTimingFrameCount(InDamageIdx, InComboIdx).DamageBeginFrameCount;
	float Result = GetFrameCountByConsideringSpeed(Begin, AttackSpeedRate);

	if (InDamageIdx != 0)
	{
		float PrevEnd = GetDamageTimingFrameCount(InDamageIdx - 1, InComboIdx).DamageEndFrameCount;
		Result = GetFrameCountByConsideringSpeed(Begin, AttackSpeedRate) - GetFrameCountByConsideringSpeed(PrevEnd, AttackSpeedRate);
	}
	return Result;
}

float UMCOActionData::GetDamageExistFrameCount(uint8 InDamageIdx, uint8 InComboIdx, float AttackSpeedRate)
{
	float BeginResult = GetDamageBeginFrameCountAfterEndTime(InDamageIdx, AttackSpeedRate, InComboIdx);
	float End = GetDamageTimingFrameCount(InDamageIdx, InComboIdx).DamageEndFrameCount;
	float EndResult = GetFrameCountByConsideringSpeed(End, AttackSpeedRate);
	
	return EndResult - BeginResult;
}

float UMCOActionData::GetFrameCountByConsideringSpeed(float FrameCount, uint8 InComboIdx, float AttackSpeedRate)
{
	return (FrameCount / GetFrameRate(InComboIdx)) / AttackSpeedRate;
}






float UMCOActionData::GetComboFrameCount(uint8 InComboIdx) const
{
	ensure(InComboIdx < AttackActionDatas.Num());
	return AttackActionDatas[InComboIdx].NextComboFrameCount;
}

float UMCOActionData::GetComboCheckTime(uint8 InComboIdx, float AttackSpeedRate)
{
	float FrameCount = GetComboFrameCount(InComboIdx);
	float FrameRate = GetFrameRate(InComboIdx);
	
	return (FrameCount / FrameRate) / AttackSpeedRate;
}