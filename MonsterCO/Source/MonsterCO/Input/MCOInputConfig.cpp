#include "Input/MCOInputConfig.h"

#include "EnhancedActionKeyMapping.h"
#include "InputMappingContext.h"

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

FText UMCOInputConfig::GetActionKeyName(const FGameplayTag& InTag) const
{
	const UInputAction* Action = FindAbilityInputActionForTag(InTag);
	
	for (const FEnhancedActionKeyMapping& Mapping : InputMappingContext->GetMappings())
	{
		if (Mapping.Action != Action)
		{
			continue;
		}

		return Mapping.Key.GetDisplayName();
	}

	return FText();
}

FKey UMCOInputConfig::GetActionKey(const FGameplayTag& InTag) const
{
	const UInputAction* Action = FindAbilityInputActionForTag(InTag);
	
	for (const FEnhancedActionKeyMapping& Mapping : InputMappingContext->GetMappings())
	{
		if (Mapping.Action != Action)
		{
			continue;
		}

		return Mapping.Key;
	}

	return FKey();
}
