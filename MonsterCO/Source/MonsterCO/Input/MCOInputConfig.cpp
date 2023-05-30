#include "Input/MCOInputConfig.h"

const UInputAction* UMCOInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMCOInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction != nullptr && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	return nullptr;
}

const UInputAction* UMCOInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMCOInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction != nullptr && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	return nullptr;
}
