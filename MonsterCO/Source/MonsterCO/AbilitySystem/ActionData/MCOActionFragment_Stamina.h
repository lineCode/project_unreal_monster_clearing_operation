#pragma once

#include "MonsterCO.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "MCOActionFragment_Stamina.generated.h"

UCLASS()
class MONSTERCO_API UMCOActionFragment_Stamina : public UMCOActionFragment
{
	GENERATED_BODY()
		
public:
	bool CanConsumeStamina() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaminaUsage = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMCOStaminaConsumptionPolicy StaminaConsumptionPolicy = EMCOStaminaConsumptionPolicy::Instant;
};
