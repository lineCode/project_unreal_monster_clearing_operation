#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"
#include "MCOActionFragment_AttributeEffect.h"
#include "MCOActionFragment_Collision.h"
#include "MCOActionFragment_Projectile.h"


bool UMCOActionFragment_AttackTiming::IsValidIdx(const uint8& InDamageIdx) const
{
	return DamageTimings.IsValidIndex(InDamageIdx);
}

bool UMCOActionFragment_AttackTiming::IsMovable(const uint8& InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	return DamageTimings[InDamageIdx].bIsMovable;
}

bool UMCOActionFragment_AttackTiming::CanTurn(const uint8& InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	return DamageTimings[InDamageIdx].bCanTurn;
}

float UMCOActionFragment_AttackTiming::GetDamageCheckRate(const uint8& InDamageIdx, const float& SpeedRate) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));

	const float GapFrameCount = DamageTimings[InDamageIdx].End - DamageTimings[InDamageIdx].Begin;
	const float SingleAttackFrameCount = GapFrameCount / DamageTimings[InDamageIdx].AttackCount;
	const float AttackCheckRate = CalculateTime(SingleAttackFrameCount, SpeedRate);
	
	return AttackCheckRate;
}

float UMCOActionFragment_AttackTiming::GetDamageBeginTimeAfterPrevEndTime(const uint8& InDamageIdx, const float& SpeedRate) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	float BeginTime = CalculateTime(DamageTimings[InDamageIdx].Begin, SpeedRate);

	if (InDamageIdx != 0)
	{
		BeginTime -= CalculateTime(DamageTimings[InDamageIdx - 1].End, SpeedRate);
	}
	
	return BeginTime;
}

float UMCOActionFragment_AttackTiming::GetDamageExistTime(const uint8& InDamageIdx, const float& SpeedRate) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));

	const float Begin = CalculateTime(DamageTimings[InDamageIdx].Begin, SpeedRate);
	const float End = CalculateTime(DamageTimings[InDamageIdx].End, SpeedRate);
	
	return End - Begin;
}

float UMCOActionFragment_AttackTiming::GetComboCheckTime(const float& SpeedRate) const
{
	return CalculateTime(NextComboFrameCount, SpeedRate);
}

float UMCOActionFragment_AttackTiming::CalculateTime(const float& FrameCount, const float& SpeedRate) const
{
	return (FrameCount / MontageFrameRate) / SpeedRate;
}

UMCOActionFragment_AttributeEffect* UMCOActionFragment_AttackTiming::GetAttributeFragment(const uint8& InDamageIdx) const
{
	return (DamageTimings.IsValidIndex(InDamageIdx)) ? DamageTimings[InDamageIdx].Attribute : nullptr;
}

UMCOActionFragment_Collision* UMCOActionFragment_AttackTiming::GetCollisionFragment(const uint8& InDamageIdx) const
{
	return (DamageTimings.IsValidIndex(InDamageIdx)) ? DamageTimings[InDamageIdx].Collision : nullptr;
}

UMCOActionFragment_Projectile* UMCOActionFragment_AttackTiming::GetProjectileFragment(const uint8& InDamageIdx) const
{
	return (DamageTimings.IsValidIndex(InDamageIdx)) ? DamageTimings[InDamageIdx].Projectile : nullptr;
}

bool UMCOActionFragment_AttackTiming::IsUsingProjectile(const uint8& InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));

	return nullptr != DamageTimings[InDamageIdx].Projectile;
}

const TSubclassOf<AMCOProjectile>* UMCOActionFragment_AttackTiming::GetProjectileClass(const uint8& InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	ISTRUE_N(DamageTimings[InDamageIdx].Projectile);
	
	return &DamageTimings[InDamageIdx].Projectile->ProjectileClass;
}

float UMCOActionFragment_AttackTiming::GetProjectileSpeed(const uint8& InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	ISTRUE_Z(DamageTimings[InDamageIdx].Projectile);
	
	return DamageTimings[InDamageIdx].Projectile->ProjectileSpeed;
}

float UMCOActionFragment_AttackTiming::GetProjectileLifeSpan(const uint8& InDamageIdx) const
{
	ensure(DamageTimings.IsValidIndex(InDamageIdx));
	ISTRUE_Z(DamageTimings[InDamageIdx].Projectile);
	
	return DamageTimings[InDamageIdx].Projectile->ProjectileLifeSpan;
}
