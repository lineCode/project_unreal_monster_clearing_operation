#include "MCOMontageDataDirectional.h"
#include "MCOActionFragment_Montage.h"


EMCOCharacterDirection UMCOMontageDataDirectional::GetDirectionFromDegree(const float InDegree, const bool bLog) const
{
	const float Gap = 45.0f;
	EMCOCharacterDirection Result = EMCOCharacterDirection::Front;
	for (int32 i = 0; i <= 4; i++)
	{
		const EMCOCharacterDirection Right = GetClosestDirectionFromDegree(InDegree + Gap * i, bLog); 
		const EMCOCharacterDirection Left = GetClosestDirectionFromDegree(InDegree - Gap * i, bLog);
		const bool HasRight = MontageFragments.Contains(Right);
		const bool HasLeft = MontageFragments.Contains(Left);

		if (false == HasRight && false == HasLeft)
		{
			continue;
		}

		Result = (true == HasRight) ? Right : Left;
		break;
	}

	if (true == bLog)
	{
		MCOLOG_C(MCOAbility, TEXT("Result direction : %f == %s"), InDegree,
			*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)Result)
		);
	}

	return Result;
}

UAnimMontage* UMCOMontageDataDirectional::GetMontage(const EMCOCharacterDirection& InDirection)
{
	ensure(MontageFragments.Contains(InDirection));
	return MontageFragments[InDirection]->Montage;
}

void UMCOMontageDataDirectional::UpdateDirectionalDefinition(UMCOActionDefinition* OutDefinition, const EMCOCharacterDirection& InDirection) const
{
	OutDefinition->CooldownFragment = GetCooldownFragment(InDirection);
	OutDefinition->AttributeFragment = GetAttributeFragment(InDirection);
	OutDefinition->AttackTimingFragment = GetAttackTimingFragment(InDirection);
	OutDefinition->CollisionFragment = GetCollisionFragment(InDirection);
}

UMCOActionFragment_Cooldown* UMCOMontageDataDirectional::GetCooldownFragment(const EMCOCharacterDirection& InDirection) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InDirection);
	if (nullptr != MontageFragment)
	{
		UMCOActionFragment_Cooldown* Fragment = MontageFragment->GetCooldownFragment();
		if (nullptr != Fragment)
		{
			return Fragment;
		}
	}
	
	return Super::GetCooldownFragment();
}

UMCOActionFragment_Attribute* UMCOMontageDataDirectional::GetAttributeFragment(const EMCOCharacterDirection& InDirection) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InDirection);
	if (nullptr != MontageFragment)
	{
		UMCOActionFragment_Attribute* Fragment = MontageFragment->GetAttributeFragment();
		if (nullptr != Fragment)
		{
			return Fragment;
		}
	}
	
	return Super::GetAttributeFragment();
}

UMCOActionFragment_AttackTiming* UMCOMontageDataDirectional::GetAttackTimingFragment(const EMCOCharacterDirection& InDirection) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InDirection);
	if (nullptr != MontageFragment)
	{
		UMCOActionFragment_AttackTiming* Fragment = MontageFragment->GetAttackTimingFragment();
		if (nullptr != Fragment)
		{
			return Fragment;
		}
	}
	
	return Super::GetAttackTimingFragment();
}

UMCOActionFragment_Collision* UMCOMontageDataDirectional::GetCollisionFragment(const EMCOCharacterDirection& InDirection) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InDirection);
	if (nullptr != MontageFragment)
	{
		UMCOActionFragment_Collision* Fragment = MontageFragment->GetCollisionFragment();
		if (nullptr != Fragment)
		{
			return Fragment;
		}
	}
	
	return Super::GetCollisionFragment();
}


UMCOActionFragment_Montage* UMCOMontageDataDirectional::GetMontageFragment(const EMCOCharacterDirection& InDirection) const
{
	ISTRUE_N(MontageFragments.Contains(InDirection));
	return MontageFragments[InDirection];
}

EMCOCharacterDirection UMCOMontageDataDirectional::GetClosestDirectionFromDegree(float InDegree, const bool bLog) const
{
	if (InDegree < -180.0f)
	{
		InDegree = 360.0f + InDegree;
	}
	else if (180.0f < InDegree)
	{
		InDegree = -360.0f + InDegree;
	}

	EMCOCharacterDirection Result;
	
	// Left
	if (InDegree < 0.0f)
	{
		if (InDegree > -22.5f)       Result = EMCOCharacterDirection::Front;
		else if (InDegree > -67.5f)  Result = EMCOCharacterDirection::Front_Left;
		else if (InDegree > -112.5f) Result = EMCOCharacterDirection::Left; 
		else if (InDegree > -157.5f) Result = EMCOCharacterDirection::Back_Left;
		else                         Result = EMCOCharacterDirection::Back;
	}
	// Right
	else
	{
		if (InDegree < 22.5f)        Result = EMCOCharacterDirection::Front;
		else if (InDegree < 67.5f)   Result = EMCOCharacterDirection::Front_Right;
		else if (InDegree < 112.5f)  Result = EMCOCharacterDirection::Right; 
		else if (InDegree < 157.5f)  Result = EMCOCharacterDirection::Back_Right;
		else                         Result = EMCOCharacterDirection::Back;
	}

	if (true == bLog)
	{
		MCOLOG_C(MCOAbility, TEXT("...Calculating : %f == %s"), InDegree,
			*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)Result)
		);
	}
	
	return Result;
}

