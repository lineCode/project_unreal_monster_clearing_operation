#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"

#include "GameplayEffect.h"
#include "MCOActionFragment_AttributeEffect.h"
#include "AbilitySystem/MCOCharacterTags.h"


bool UMCOActionFragment_AttackTiming::IsMovable(const uint8& InDamageIdx) const
{
	ISTRUE_F(DamageTimings.IsValidIndex(InDamageIdx));
	return DamageTimings[InDamageIdx].bIsMovable;
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

float UMCOActionFragment_AttackTiming::CalculateTime(const float& FrameCount, const float& SpeedRate) const
{
	return (FrameCount / MontageFrameRate) / SpeedRate;
}

UMCOActionFragment_AttributeEffect* UMCOActionFragment_AttackTiming::GetAttributeFragment(const uint8& InDamageIdx) const
{
	return (DamageTimings.IsValidIndex(InDamageIdx)) ? DamageTimings[InDamageIdx].Attribute : nullptr;
}

UNiagaraSystem* UMCOActionFragment_AttackTiming::GetDamageNiagara(const uint8& InDamageIdx) const
{
	ISTRUE_N(DamageTimings.IsValidIndex(InDamageIdx))
	ISTRUE_N(nullptr != DamageTimings[InDamageIdx].Attribute);
	
	return DamageTimings[InDamageIdx].Attribute->GetNiagara(EMCOEffectPolicy::Instant);
}

UNiagaraSystem* UMCOActionFragment_AttackTiming::GetDurationEffectNiagara(const uint8& InDamageIdx) const
{
	ISTRUE_N(DamageTimings.IsValidIndex(InDamageIdx))
	ISTRUE_N(nullptr != DamageTimings[InDamageIdx].Attribute);
	
	return DamageTimings[InDamageIdx].Attribute->GetNiagara(EMCOEffectPolicy::Duration);
}

bool UMCOActionFragment_AttackTiming::IsUsingProjectile(const uint8& InDamageIdx) const
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
