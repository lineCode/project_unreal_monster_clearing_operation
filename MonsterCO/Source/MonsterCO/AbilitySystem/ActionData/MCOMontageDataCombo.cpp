#include "MCOMontageDataCombo.h"
#include "MCOActionFragment_Montage.h"


UAnimMontage* UMCOMontageDataCombo::GetMontage(const uint8& InComboIdx)
{
	ensure(MontageFragments.IsValidIndex(InComboIdx));
	return MontageFragments[InComboIdx]->Montage;
}

uint8 UMCOMontageDataCombo::GetMaxCombo() const
{
	return MontageFragments.Num();
}

void UMCOMontageDataCombo::UpdateComboDefinition(UMCOActionDefinition* OutDefinition, const uint8& InComboIdx) const
{
	OutDefinition->CooldownFragment = GetCooldownFragment(InComboIdx);
	OutDefinition->AttributeFragment = GetAttributeFragment(InComboIdx);
	OutDefinition->AttackTimingFragment = GetAttackTimingFragment(InComboIdx);
	OutDefinition->CollisionFragment = GetCollisionFragment(InComboIdx, 0);
}

UMCOActionFragment_Cooldown* UMCOMontageDataCombo::GetCooldownFragment(const uint8& InComboIdx) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InComboIdx);
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

UMCOActionFragment_AttributeEffect* UMCOMontageDataCombo::GetAttributeFragment(const uint8& InComboIdx) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InComboIdx);
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

UMCOActionFragment_AttackTiming* UMCOMontageDataCombo::GetAttackTimingFragment(const uint8& InComboIdx) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InComboIdx);
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

UMCOActionFragment_Collision* UMCOMontageDataCombo::GetCollisionFragment(const uint8& InComboIdx, const uint8& InDamageIdx) const
{
	const UMCOActionFragment_Montage* MontageFragment = GetMontageFragment(InComboIdx);
	if (nullptr != MontageFragment)
	{
		UMCOActionFragment_Collision* Fragment = MontageFragment->GetCollisionFragment(InDamageIdx);
		if (nullptr != Fragment)
		{
			return Fragment;
		}
	}
	
	return Super::GetCollisionFragment();
}

UMCOActionFragment_Montage* UMCOMontageDataCombo::GetMontageFragment(const uint8& InComboIdx) const
{
	ISTRUE_N(MontageFragments.IsValidIndex(InComboIdx));
	return MontageFragments[InComboIdx];
}
