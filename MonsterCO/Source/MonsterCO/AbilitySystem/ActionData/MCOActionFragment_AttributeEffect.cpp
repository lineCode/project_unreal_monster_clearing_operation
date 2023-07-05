#include "AbilitySystem/ActionData/MCOActionFragment_AttributeEffect.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "GameplayEffect.h"


bool UMCOActionFragment_AttributeEffect::CanActivateAbilityWithStamina(const float& CurrentStamina) const
{
	for (auto Tuple : Data)
	{
		if (Tuple.Value.bIsStartupEffectOfAbility == false)
		{
			continue;
		}
		
		const float StaminaConsumptionAmount = Tuple.Value.Stamina;
		return 0.0f <= StaminaConsumptionAmount + CurrentStamina;
	}

	return true;
}

bool UMCOActionFragment_AttributeEffect::IsEffectExistByPolicy(const EMCOEffectPolicy& InPolicy) const
{
	return Data.Contains(InPolicy);
}

void UMCOActionFragment_AttributeEffect::ApplyAttributeAdditiveValue(const EMCOEffectPolicy& InPolicy, const FGameplayEffectSpecHandle& InHandle) const
{
	ISTRUE(true == Data.Contains(InPolicy));
	
	const FMCOAttributeValue& AttributeValue = Data[InPolicy];

	FGameplayTagContainer Tags;
	
	if (0.0f != AttributeValue.Stamina)
	{
		InHandle.Data->SetSetByCallerMagnitude(
			FMCOCharacterTags::Get().GameplayEffect_StaminaTag,
			AttributeValue.Stamina
		);

		Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StaminaTag);
	}
	
	if (0.0f != AttributeValue.Health)
	{
		InHandle.Data->SetSetByCallerMagnitude(
			FMCOCharacterTags::Get().GameplayEffect_HealthTag,
			AttributeValue.Health
		);

		Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_HealthTag);
	}
	
	if (0.0f != AttributeValue.Damage)
	{
		InHandle.Data->SetSetByCallerMagnitude(
			FMCOCharacterTags::Get().GameplayEffect_DamageTag,
			AttributeValue.Damage
		);

		Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_DamageTag);
	}

	if (0.0f != AttributeValue.Stiffness)
	{
		InHandle.Data->SetSetByCallerMagnitude(
			FMCOCharacterTags::Get().GameplayEffect_StiffnessTag,
			AttributeValue.Stiffness
		);

		Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag);
	}
	
	if (InPolicy == EMCOEffectPolicy::Duration)
	{
		InHandle.Data->SetSetByCallerMagnitude(
			FMCOCharacterTags::Get().GameplayEffect_DurationTag,
			AttributeValue.Duration
		);
	}

	if (false == AttributeValue.bIsStartupEffectOfAbility)
	{
		Tags.AddTag(FMCOCharacterTags::Get().EffectRemoveOnDeathTag);
	}
	
	InHandle.Data->DynamicGrantedTags = Tags;

	// MCOLOG_C(MCOAbility, TEXT("Effect Applied [Stamina %.1f] [Health %.1f] [Damage %.1f] [Stiffness %.1f]"),
	// 	AttributeValue.Stamina, AttributeValue.Health, AttributeValue.Damage, AttributeValue.Stiffness
	// );
}

FGameplayTagContainer UMCOActionFragment_AttributeEffect::GetAllTags() const
{
	FGameplayTagContainer Tags;

	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StaminaTag);
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_HealthTag);
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_DamageTag);
	Tags.AddTag(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag);
	
	return Tags;
}

float UMCOActionFragment_AttributeEffect::GetDamage(const EMCOEffectPolicy& InPolicy) const
{
	ISTRUE_Z(true == Data.Contains(InPolicy));
	
	return Data[InPolicy].Damage;
}