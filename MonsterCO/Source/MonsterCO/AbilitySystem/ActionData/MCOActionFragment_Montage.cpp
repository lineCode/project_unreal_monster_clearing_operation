#include "AbilitySystem/ActionData/MCOActionFragment_Montage.h"
#include "MCOActionDefinition.h"


UMCOActionFragment_Cooldown* UMCOActionFragment_Montage::GetCooldownFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->CooldownFragment : nullptr;
}

UMCOActionFragment_Attribute* UMCOActionFragment_Montage::GetAttributeFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->AttributeFragment : nullptr;
}

UMCOActionFragment_AttackTiming* UMCOActionFragment_Montage::GetAttackTimingFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->AttackTimingFragment : nullptr;
}

UMCOActionFragment_Collision* UMCOActionFragment_Montage::GetCollisionFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->CollisionFragment : nullptr;
}

