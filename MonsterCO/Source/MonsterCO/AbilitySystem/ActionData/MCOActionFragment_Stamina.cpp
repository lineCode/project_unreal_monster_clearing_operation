#include "AbilitySystem/ActionData/MCOActionFragment_Stamina.h"

bool UMCOActionFragment_Stamina::CanConsumeStamina() const
{
	return StaminaUsage != 0.0f;
}
