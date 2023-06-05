#pragma once

#include "MonsterCO.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "MCOAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Delegate used to broadcast attribute events.
DECLARE_MULTICAST_DELEGATE_FiveParams(FMCOHandleAttributeEventDelegate,
	const FGameplayTag& /*InTag*/, 
	AActor* /*EffectInstigator*/,
	AActor* /*EffectCauser*/,
	const FGameplayEffectSpec& /*EffectSpec*/,
	float /*EffectMagnitude*/
);

UCLASS()
class MONSTERCO_API UMCOAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UMCOAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, MaxStamina)
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, Damage)
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, Stiffness)
	ATTRIBUTE_ACCESSORS(UMCOAttributeSet, MaxStiffness)

	mutable FMCOHandleAttributeEventDelegate OnHandleAttributeEventDelegate;
		
private:
	UPROPERTY(BlueprintReadOnly, Category = "MCO|Health", ReplicatedUsing = OnRep_Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, Category = "MCO|MaxHealth", ReplicatedUsing = OnRep_MaxHealth, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "MCO|Stamina", ReplicatedUsing = OnRep_Stamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	
	UPROPERTY(BlueprintReadOnly, Category = "MCO|MaxStamina", ReplicatedUsing = OnRep_MaxStamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	
	// Damage is a meta attribute used by the DamageExecution to calculate final damage, 
	// which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "MCO|Damage", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	
	UPROPERTY(BlueprintReadOnly, Category = "MCO|Stiffness", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Stiffness;
	
	UPROPERTY(BlueprintReadOnly, Category = "MCO|MaxStiffness", ReplicatedUsing = OnRep_MaxStiffness, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStiffness;

protected:
	// Functions for replication
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStiffness(const FGameplayAttributeData& OldMaxStiffness);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	void HandleEventWithTag(const FGameplayTag& InTag, const FGameplayEffectModCallbackData& Data) const;
};
