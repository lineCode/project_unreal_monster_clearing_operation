#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MCOAttackDefinition.h"
#include "MCOAttackFragment_Cooldown.generated.h"

UCLASS()
class MONSTERCO_API UMCOAttackFragment_Cooldown : public UMCOAttackFragment
{
	GENERATED_BODY()
	
public:
	bool CanApplyCooldown() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer CooldownTags;
};
