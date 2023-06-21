#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Cooldown.generated.h"



UCLASS()
class MONSTERCO_API UMCOActionFragment_Cooldown : public UMCOActionFragment
{
	GENERATED_BODY()
	
public:
	bool CanApplyCooldown() const;
	void ApplyCooldownValue(const FGameplayEffectSpecHandle& InHandle) const;
	const FGameplayTagContainer* GetCooldownTags() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CooldownTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer CooldownTags;
};
