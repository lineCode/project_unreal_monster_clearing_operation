#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"


bool UMCOActionFragment_AttackTiming::IsMovableWhileGivingDamage(const uint8& InDamageIdx) const
{
	ISTRUE_F(DamageTimings.IsValidIndex(InDamageIdx));
	return DamageTimings[InDamageIdx].bIsMovableWhileGivingDamage;
}

float UMCOActionFragment_AttackTiming::GetDamageCheckRate(const uint8& InDamageIdx) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	return DamageTimings[InDamageIdx].CheckRate;
}

float UMCOActionFragment_AttackTiming::GetDamageBeginTimeAfterPrevEndTime(const uint8& InDamageIdx, const float& SpeedRate) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	float BeginTime = CalculateTime(DamageTimings[InDamageIdx].Begin, SpeedRate);

	if (InDamageIdx != 0)
	{
		BeginTime = CalculateTime(DamageTimings[InDamageIdx].Begin, SpeedRate) -
			CalculateTime(DamageTimings[InDamageIdx - 1].End, SpeedRate);
	}
	return BeginTime;
}

float UMCOActionFragment_AttackTiming::GetDamageExistTime(const uint8& InDamageIdx, const float& SpeedRate) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx))

	float Begin = GetDamageBeginTimeAfterPrevEndTime(InDamageIdx, SpeedRate);
	float End = CalculateTime(DamageTimings[InDamageIdx].End, SpeedRate);
	
	return End - Begin;
}

float UMCOActionFragment_AttackTiming::GetComboCheckTime(const float& SpeedRate) const
{
	return CalculateTime(NextComboFrameCount, SpeedRate);
}

UNiagaraSystem* UMCOActionFragment_AttackTiming::GetDamageNiagara(const uint8& InDamageIdx) const
{
	ISTRUE_N(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].DamagedNiagara;
}

bool UMCOActionFragment_AttackTiming::IsAttackByProjectile(const uint8& InDamageIdx) const
{
	return DamageTimings[InDamageIdx].bUseProjectile;
}

const TSubclassOf<AMCOProjectile>* UMCOActionFragment_AttackTiming::GetProjectileClass(const uint8& InDamageIdx) const
{
	ISTRUE_N(DamageTimings.IsValidIndex(InDamageIdx));
	
	return &DamageTimings[InDamageIdx].ProjectileClass;
}

float UMCOActionFragment_AttackTiming::GetProjectileSpeed(const uint8& InDamageIdx) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].ProjectileSpeed;
}

float UMCOActionFragment_AttackTiming::GetProjectileLifeSpan(const uint8& InDamageIdx) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].ProjectileLifeSpan;
}

float UMCOActionFragment_AttackTiming::GetStiffness(const uint8& InDamageIdx) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].Stiffness;
}

float UMCOActionFragment_AttackTiming::GetDamage(const uint8& InDamageIdx) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].Damage;
}

float UMCOActionFragment_AttackTiming::GetDamageDurationTime(const uint8& InDamageIdx) const
{
	ISTRUE_Z(DamageTimings.IsValidIndex(InDamageIdx));
	
	return DamageTimings[InDamageIdx].DurationTime;
}

float UMCOActionFragment_AttackTiming::CalculateTime(const float& FrameCount, const float& SpeedRate) const
{
	return (FrameCount / MontageFrameRate) / SpeedRate;
}
