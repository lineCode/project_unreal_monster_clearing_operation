#pragma once

#include "MonsterCO.h"
#include "EnhancedInputComponent.h"
#include "Input/MCOInputConfig.h"
#include "InputTriggers.h"
#include "GameplayTagContainer.h"
#include "MCOInputComponent.generated.h"

UCLASS()
class MONSTERCO_API UMCOInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	void SetMappingContext(const UMCOInputConfig* InputConfig, class UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UMCOInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMCOInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
};


template<class UserClass, typename FuncType>
void UMCOInputComponent::BindNativeAction(const UMCOInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	ensure(InputConfig);
	
	const UInputAction* InputAction = InputConfig->FindNativeInputActionForTag(InputTag);
	ISTRUE(InputAction);
	
	BindAction(InputAction, TriggerEvent, Object, Func);
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMCOInputComponent::BindAbilityActions(const UMCOInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	ensure(InputConfig);
	
	for (const FMCOInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction == nullptr || Action.InputTag.IsValid() == false) continue;
		
		if (PressedFunc != nullptr)
		{
			BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag);
		}

		if (ReleasedFunc != nullptr)
		{
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
		}
	}
}