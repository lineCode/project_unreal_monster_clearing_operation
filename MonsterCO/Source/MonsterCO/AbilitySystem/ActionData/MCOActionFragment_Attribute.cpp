#include "AbilitySystem/ActionData/MCOActionFragment_Attribute.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "GameplayEffect.h"


float UMCOActionFragment_Attribute::GetHealthAdditiveValue() const
{
	ISTRUE_Z(true == AdditiveValues.Contains(FMCOCharacterTags::Get().GameplayEffect_HealthTag));

	return AdditiveValues[FMCOCharacterTags::Get().GameplayEffect_HealthTag].AdditiveValue;
}

float UMCOActionFragment_Attribute::GetStaminaAdditiveValue() const
{
	ISTRUE_Z(true == AdditiveValues.Contains(FMCOCharacterTags::Get().GameplayEffect_StaminaTag));

	return AdditiveValues[FMCOCharacterTags::Get().GameplayEffect_StaminaTag].AdditiveValue;
}

float UMCOActionFragment_Attribute::GetStiffnessAdditiveValue() const
{
	ISTRUE_Z(true == AdditiveValues.Contains(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag));

	return AdditiveValues[FMCOCharacterTags::Get().GameplayEffect_StiffnessTag].AdditiveValue;
}

bool UMCOActionFragment_Attribute::CanApply(const FGameplayTag& InTag) const
{
	ISTRUE_F(true == AdditiveValues.Contains(InTag));
	
	return AdditiveValues[InTag].AdditiveValue != 0.0f;
}

bool UMCOActionFragment_Attribute::ApplyAttributeAdditiveValue(const FGameplayEffectSpecHandle& InInstantHandle,const FGameplayEffectSpecHandle& InInfiniteHandle, const FGameplayTag& InTag) const
{
	ISTRUE_F(true == CanApply(InTag));
	
	//MCOLOG(TEXT("Apply [%s] + [%f]"), *InTag.GetTagName().ToString(), AdditiveValues[InTag].AdditiveValue);

	const FGameplayEffectSpecHandle& Handle = (AdditiveValues[InTag].StaminaConsumptionPolicy == EMCOStaminaConsumptionPolicy::Instant) ? InInstantHandle : InInfiniteHandle;
	
	Handle.Data->SetSetByCallerMagnitude(
		InTag,
		AdditiveValues[InTag].AdditiveValue
	);

	FGameplayTagContainer Tags;
	Tags.AddTag(InTag);
	Handle.Data->DynamicGrantedTags = Tags;

	return true;
}
