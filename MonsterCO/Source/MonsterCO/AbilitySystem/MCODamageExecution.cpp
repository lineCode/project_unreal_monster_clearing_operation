#include "MCODamageExecution.h"
#include "AbilitySystemComponent.h"
#include "MCOAttributeSet.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOCharacterTags.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition AdditiveHealthDef;
	FGameplayEffectAttributeCaptureDefinition AdditiveStaminaDef;
	FGameplayEffectAttributeCaptureDefinition AdditiveStiffnessDef;
	FGameplayEffectAttributeCaptureDefinition AdditiveDamageDef;

	FDamageStatics()
	{
		// Capture optional Damage set on the damage GE as a Calculation Modifier under the ExecutionCalculation
		AdditiveHealthDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		AdditiveStaminaDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		AdditiveStiffnessDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveStiffnessAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		AdditiveDamageDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UMCODamageExecution::UMCODamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveStaminaDef);
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveStiffnessDef);
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveDamageDef);
}

// calculate damage and modify health 
void UMCODamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// get ability system components of source & target 
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	// get GameplayEffect Spec to use its variables and tags
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AdditiveDamage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveDamageDef, EvaluationParameters, AdditiveDamage);
	// Add SetByCaller damage if it exists
	AdditiveDamage += Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_DamageTag, false, 0.0f);
	
	float AdditiveHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveHealthDef, EvaluationParameters, AdditiveHealth);
	AdditiveHealth = Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_HealthTag, false, 0.0f);

	float AdditiveStamina = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveStaminaDef, EvaluationParameters, AdditiveStamina);
	AdditiveStamina = Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_StaminaTag, false, 0.0f);

	float AdditiveStiffness = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveStiffnessDef, EvaluationParameters, AdditiveStiffness);
	AdditiveStiffness = Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag, false, 0.0f);

	
	// Check Dodge
	if (AdditiveDamage < 0.0f || AdditiveHealth < 0.0f)
	{		
		if (true == TargetASC->HasMatchingGameplayTag(FMCOCharacterTags::Get().DodgeTag))
		{
			MCOPRINT(TEXT("Escaped attack by dodging"));
			return;
		}
	}

	if (0.0f < AdditiveStiffness)
	{
		UMCOAbilitySystemComponent* TargetMCOASC = Cast<UMCOAbilitySystemComponent>(TargetASC);
		UMCOAbilitySystemComponent* SourceMCOASC = Cast<UMCOAbilitySystemComponent>(SourceASC);
		ISTRUE(nullptr != TargetASC);
		ISTRUE(nullptr != SourceMCOASC);
		
		// Broadcast damages to Target ASC
		TargetMCOASC->ReceiveDamage(SourceMCOASC, AdditiveDamage, TargetMCOASC->AttributeSet->GetStiffness() + AdditiveStiffness);
	}
	
	if (AdditiveDamage != 0.0f)
	{
		// Set the Target's Damage meta attribute
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UMCOAttributeSet::GetAdditiveDamageAttribute(), 
				EGameplayModOp::Additive, 
				AdditiveDamage
			)
		);
	}
	if (AdditiveHealth != 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UMCOAttributeSet::GetAdditiveHealthAttribute(), 
				EGameplayModOp::Additive, 
				AdditiveHealth
			)
		);
	}
	if (AdditiveStamina != 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UMCOAttributeSet::GetAdditiveStaminaAttribute(), 
				EGameplayModOp::Additive, 
				AdditiveStamina
			)
		);
	}
	if (AdditiveStiffness != 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UMCOAttributeSet::GetAdditiveStiffnessAttribute(), 
				EGameplayModOp::Additive, 
				AdditiveStiffness
			)
		);
	}

	
}
