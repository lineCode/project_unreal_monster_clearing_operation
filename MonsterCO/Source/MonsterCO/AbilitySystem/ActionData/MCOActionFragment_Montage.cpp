#include "AbilitySystem/ActionData/MCOActionFragment_Montage.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_AttackTiming.h"


UMCOActionFragment_Cooldown* UMCOActionFragment_Montage::GetCooldownFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->CooldownFragment : nullptr;
}

UMCOActionFragment_AttributeEffect* UMCOActionFragment_Montage::GetAttributeFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->AttributeFragment : nullptr;
}

UMCOActionFragment_AttackTiming* UMCOActionFragment_Montage::GetAttackTimingFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->AttackTimingFragment : nullptr;
}

UMCOActionFragment_Collision* UMCOActionFragment_Montage::GetCollisionFragment(const uint8& InDamageIdx) const
{
	ISTRUE_N(nullptr != ActionDefinition);

	if (nullptr != ActionDefinition->AttackTimingFragment)
	{
		UMCOActionFragment_Collision* Fragment = ActionDefinition->AttackTimingFragment->GetCollisionFragment(InDamageIdx);
		if (nullptr != Fragment)
		{
			return Fragment;
		}
	}

	return ActionDefinition->CollisionFragment;
}
