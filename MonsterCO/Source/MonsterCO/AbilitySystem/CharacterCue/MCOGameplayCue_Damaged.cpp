#include "AbilitySystem/CharacterCue/MCOGameplayCue_Damaged.h"


AMCOGameplayCue_Damaged::AMCOGameplayCue_Damaged()
{
	
}

void AMCOGameplayCue_Damaged::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	MCOPRINT(TEXT("%s"), *MyTarget->GetName());

	
}
