#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "MCOActionFragment_Attribute.generated.h"



USTRUCT(BlueprintType)
struct FMCOAttributeValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EMCOStaminaConsumptionPolicy StaminaConsumptionPolicy = EMCOStaminaConsumptionPolicy::Instant;

	UPROPERTY(EditAnywhere)
	float AdditiveValue = 0;
};



UCLASS()
class MONSTERCO_API UMCOActionFragment_Attribute : public UMCOActionFragment
{
	GENERATED_BODY()
		
public:
	float GetHealthAdditiveValue() const;
	float GetStaminaAdditiveValue() const;
	float GetStiffnessAdditiveValue() const;
	bool CanApply(const FGameplayTag& InTag) const;
	bool ApplyAttributeAdditiveValue(const FGameplayEffectSpecHandle& InInstantHandle,const FGameplayEffectSpecHandle& InInfiniteHandle, const FGameplayTag& InTag) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FMCOAttributeValue> AdditiveValues;
};
