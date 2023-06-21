#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "MCOActionFragment_AttributeEffect.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FMCOAttributeValue
{
	GENERATED_BODY()

public:
	FMCOAttributeValue() : bIsStartupStamina(false) {}
	
	UPROPERTY(EditAnywhere)
	uint8 bIsStartupStamina : 1;
	
	UPROPERTY(EditAnywhere)
	float Stamina = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float Health = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float Damage = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float Stiffness = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> Niagara = nullptr;
	
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
	UNiagaraSystem* GetNiagara(const EMCOEffectPolicy& InPolicy) const;

protected:
	UPROPERTY(EditAnywhere)
	TMap<EMCOEffectPolicy, FMCOAttributeValue> Data;

};
