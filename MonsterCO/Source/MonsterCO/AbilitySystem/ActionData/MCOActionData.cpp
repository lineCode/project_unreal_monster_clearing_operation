#include "AbilitySystem/ActionData/MCOActionData.h"
#include "MCOActionDefinition.h"


void UMCOActionData::UpdateDefinition(UMCOActionDefinition* OutDefinition) const
{
	OutDefinition->CooldownFragment = GetCooldownFragment();
	OutDefinition->AttributeFragment = GetAttributeFragment();
	OutDefinition->AttackTimingFragment = GetAttackTimingFragment();
	OutDefinition->CollisionFragment = GetCollisionFragment();
}

UMCOActionFragment_Cooldown* UMCOActionData::GetCooldownFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->CooldownFragment : nullptr;
}

UMCOActionFragment_AttributeEffect* UMCOActionData::GetAttributeFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->AttributeFragment : nullptr;
}

UMCOActionFragment_AttackTiming* UMCOActionData::GetAttackTimingFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->AttackTimingFragment : nullptr;
}

UMCOActionFragment_Collision* UMCOActionData::GetCollisionFragment() const
{
	return (nullptr != ActionDefinition) ? ActionDefinition->CollisionFragment : nullptr;
}

