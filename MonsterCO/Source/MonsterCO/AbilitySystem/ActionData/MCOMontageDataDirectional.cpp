#include "MCOMontageDataDirectional.h"
#include "MCOActionFragment_Montage.h"


EMCOCharacterDirection UMCOMontageDataDirectional::GetDirectionFromDegree(const float InDegree, const bool bLog) const
{
	const float Gap = 45.0f;
	EMCOCharacterDirection Result = EMCOCharacterDirection::Front;
	for (int32 i = 0; i <= 4; i++)
	{
		EMCOCharacterDirection Right;
		const float RightGap = GetClosestDirectionFromDegree(InDegree + Gap * i, Right, bLog); 
		EMCOCharacterDirection Left;
		const float LeftGap = GetClosestDirectionFromDegree(InDegree - Gap * i, Left, bLog);
		const bool HasRight = MontageFragments.Contains(Right);
		const bool HasLeft = MontageFragments.Contains(Left);

		if (false == HasRight && false == HasLeft)
		{
			continue;
		}
		else if (true == HasRight && true == HasLeft)
		{
			Result = (RightGap < LeftGap) ? Right : Left; 
		}
		else
		{
			Result = (true == HasRight) ? Right : Left;
		}
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
	ensure(nullptr != OutDefinition);

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

UMCOActionFragment_AttributeEffect* UMCOMontageDataDirectional::GetAttributeFragment(const EMCOCharacterDirection& InDirection) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InDirection);
	if (nullptr != MontageFragment)
	{
		UMCOActionFragment_AttributeEffect* Fragment = MontageFragment->GetAttributeFragment();
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

float UMCOMontageDataDirectional::GetClosestDirectionFromDegree(float InDegree, EMCOCharacterDirection& OutDirection, const bool bLog) const
{
	if (InDegree < -180.0f)
	{
		InDegree = 360.0f + FMath::Fmod(InDegree, 360.0f);
	}
	else if (180.0f < InDegree)
	{
		InDegree = -360.0f + FMath::Fmod(InDegree, 360.0f);
	}

	const int32 MaxDirectionCount = static_cast<int32>(EMCOCharacterDirection::Max);
	const float GapDegree = 360.0f / MaxDirectionCount;
	const float StartDegree = GapDegree / 2;
	
	float ResultGapDegree = 0.0f;
	int32 ResultIndex = 0;
	
	// Left
	if (InDegree < 0.0f)
	{
		for (int32 i = 0; i <= 4; i++)
		{
			const float CurrentDegree = -StartDegree - GapDegree * i;
			if (InDegree > CurrentDegree) // -22.5f > -67.5 > -112.5 > 157.5 > 202.5
			{
				ResultIndex = i; // front > fl > l > bl > back
				ResultGapDegree = InDegree - CurrentDegree;
				break;
			}
		}
	}
	// Right
	else
	{
		for (int32 i = 0; i <= 4; i++)
		{
			const float CurrentDegree = StartDegree + GapDegree * i;
			if (InDegree < CurrentDegree)
			{
				ResultIndex = i; // front > fr > r > br > back
				ResultGapDegree = InDegree - CurrentDegree;
				break;
			}
		}

		if (ResultIndex != 0)
		{
			ResultIndex = MaxDirectionCount - ResultIndex;
		}
	}

	OutDirection = static_cast<EMCOCharacterDirection>(ResultIndex);

	if (true == bLog)
	{
		MCOLOG_C(MCOAbility, TEXT("...Calculating : %f == %s"), InDegree,
			*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)OutDirection)
		);
	}
	
	return FMath::Abs(ResultGapDegree);
}

