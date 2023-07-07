#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "MCOGameplayCue_Static.generated.h"

UCLASS()
class MONSTERCO_API UMCOGameplayCue_Static : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
protected:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
};
