#include "Input/MCOInputComponent.h"
#include "EnhancedInputSubsystems.h"


void UMCOInputComponent::SetMappingContext(const UMCOInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	ensure(InputConfig);
	ensure(InputSubsystem);
	ensure(InputConfig->InputMappingContext);
	
	InputSubsystem->AddMappingContext(InputConfig->InputMappingContext, 0);
}
