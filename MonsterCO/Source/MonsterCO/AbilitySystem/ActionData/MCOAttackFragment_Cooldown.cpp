#include "AbilitySystem/ActionData/MCOAttackFragment_Cooldown.h"

bool UMCOAttackFragment_Cooldown::CanApplyCooldown() const
{
	return CooldownTime > 0.0f && CooldownTags.IsEmpty() == false;
}
