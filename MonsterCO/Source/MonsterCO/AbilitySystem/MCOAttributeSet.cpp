#include "MCOAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "MCOCharacterTags.h"

UMCOAttributeSet::UMCOAttributeSet()
	: Health(0.0f)
	, MaxHealth(0.0f)
	, Stamina(0.0f)
	, MaxStamina(0.0f)
	, Stiffness(0.0f)
	, MaxStiffness(0.0f)
	, AdditiveHealth(0.0f)
	, AdditiveStamina(0.0f)
	, AdditiveStiffness(0.0f)
	, AdditiveDamage(0.0f)
{
}

void UMCOAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCOAttributeSet, Health, OldHealth);
}

void UMCOAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCOAttributeSet, MaxHealth, OldMaxHealth);
}

void UMCOAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCOAttributeSet, Stamina, OldStamina);
}

void UMCOAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCOAttributeSet, MaxStamina, OldMaxStamina);
}

void UMCOAttributeSet::OnRep_Stiffness(const FGameplayAttributeData& OldStiffness)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCOAttributeSet, Stiffness, OldStiffness);
}

void UMCOAttributeSet::OnRep_MaxStiffness(const FGameplayAttributeData& OldMaxStiffness)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMCOAttributeSet, MaxStiffness, OldMaxStiffness);
}

void UMCOAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttributeSet, Stiffness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCOAttributeSet, MaxStiffness, COND_None, REPNOTIFY_Always);
}

// On Effect applied
bool UMCOAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UMCOAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	ISTRUE(Data.EvaluatedData.Magnitude != 0.0f);

	// Set{Attribute}() will call bound function in PlayerState
	
	// Additive attributes
	if (Data.EvaluatedData.Attribute == GetAdditiveDamageAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth() + GetAdditiveDamage(), 0.0f, GetMaxHealth());
		SetHealth(NewHealth);

		if (GetHealth() <= 0.0f)
		{
			HandleEventWithTag(FMCOCharacterTags::Get().GameplayEvent_DeadTag, Data);
		}
		
		SetAdditiveDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetAdditiveHealthAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth() + GetAdditiveHealth(), 0.0f, GetMaxHealth());
		SetHealth(NewHealth);		
		SetAdditiveHealth(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetAdditiveStiffnessAttribute())
	{
		float NewStiffness = GetStiffness() + GetAdditiveStiffness();

		if (GetMaxStiffness() <= NewStiffness)
		{
			NewStiffness -= GetMaxStiffness();
			HandleEventWithTag(FMCOCharacterTags::Get().GameplayEvent_DamagedTag, Data);
		}
		
		SetStiffness(NewStiffness);
		SetAdditiveStiffness(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetAdditiveStaminaAttribute())
	{
		const float NewStamina = FMath::Clamp(GetStamina() + GetAdditiveStamina(), 0.0f, GetMaxStamina());
		SetStamina(NewStamina);
		
		if (GetMaxStamina() <= GetStamina())
		{
			MCOLOG(TEXT("--------------------------Cancel stamina charging ability with tag because stamina is full"));
		
			FGameplayTagContainer Tags;
			Tags.AddTag(FMCOCharacterTags::Get().ChargingTag);
			GetOwningAbilitySystemComponent()->CancelAbilities(&Tags);
		}
		// else if (GetAdditiveStamina() < 0.0f) 
		// {
		// 	HandleEventWithTag(FMCOCharacterTags::Get().GameplayEvent_StaminaChargeTag, Data);
		// }
		
		SetAdditiveStamina(0.0f);
	}
	
}

void UMCOAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UMCOAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UMCOAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	
}

void UMCOAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, (GetMaxHealth() != 0.0f ? GetMaxHealth() : 100.0f));
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, (GetMaxStamina() != 0.0f ? GetMaxStamina() : 100.0f));
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetStiffnessAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, (GetMaxStamina() != 0.0f ? GetMaxStamina() : 100.0f));
	}
	else if (Attribute == GetMaxStiffnessAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UMCOAttributeSet::HandleEventWithTag(const FGameplayTag& InTag, const FGameplayEffectModCallbackData& Data) const
{
	ISTRUE(true == OnHandleAttributeEventDelegate.IsBound());
	
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();

	OnHandleAttributeEventDelegate.Broadcast(
		InTag, Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude
	);
}
