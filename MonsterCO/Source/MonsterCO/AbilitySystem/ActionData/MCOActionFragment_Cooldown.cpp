#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"

bool UMCOActionFragment_Cooldown::CanApplyCooldown() const
{
	return CooldownTime > 0.0f && CooldownTags.IsEmpty() == false;
}
