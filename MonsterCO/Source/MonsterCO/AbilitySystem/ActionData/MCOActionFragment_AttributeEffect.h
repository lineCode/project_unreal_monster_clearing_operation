#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_AttributeEffect.generated.h"



class UNiagaraSystem;



USTRUCT(BlueprintType)
struct FMCOAttributeValue
{
	GENERATED_BODY()

public:
	FMCOAttributeValue() : bIsStartupEffectOfAbility(false) {}
	
	UPROPERTY(EditAnywhere)
	uint8 bIsStartupEffectOfAbility : 1; 
	
	UPROPERTY(EditAnywhere)
	float Stamina = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float Health = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float Damage = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float Stiffness = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float Duration = 0.0f;
};


UCLASS()
class MONSTERCO_API UMCOActionFragment_AttributeEffect : public UMCOActionFragment
{
	GENERATED_BODY()

public:
	bool CanActivateAbilityWithStamina(const float& CurrentStamina) const;
	bool IsEffectExistByPolicy(const EMCOEffectPolicy& InPolicy) const;
	void ApplyAttributeAdditiveValue(const EMCOEffectPolicy& InPolicy, const FGameplayEffectSpecHandle& InHandle) const;
	FGameplayTagContainer GetAllTags() const;
	float GetDamage(const EMCOEffectPolicy& InPolicy) const;

protected:
	UPROPERTY(EditAnywhere)
	TMap<EMCOEffectPolicy, FMCOAttributeValue> Data;

};
