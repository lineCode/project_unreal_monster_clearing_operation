#include "AbilitySystem/MCOAttributeExecution.h"
#include "AbilitySystemComponent.h"
#include "MCOAttributeSet.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOCharacterTags.h"


struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition AdditiveHealthDef;
	FGameplayEffectAttributeCaptureDefinition AdditiveStaminaDef;
	FGameplayEffectAttributeCaptureDefinition AdditiveStiffnessDef;

	FDamageStatics()
	{
		// Capture optional Damage set on the damage GE as a Calculation Modifier under the ExecutionCalculation
		AdditiveHealthDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		AdditiveStaminaDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		AdditiveStiffnessDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetAdditiveStiffnessAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UMCOAttributeExecution::UMCOAttributeExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveStaminaDef);
	RelevantAttributesToCapture.Add(DamageStatics().AdditiveStiffnessDef);
}

void UMCOAttributeExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	
	float AdditiveHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveHealthDef, EvaluationParameters, AdditiveHealth);
	AdditiveHealth = Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_HealthTag, false, 0.0f);

	float AdditiveStamina = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveStaminaDef, EvaluationParameters, AdditiveStamina);
	AdditiveStamina = Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_StaminaTag, false, 0.0f);
	
	float AdditiveStiffness = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AdditiveStiffnessDef, EvaluationParameters, AdditiveStiffness);
	AdditiveStiffness = Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag, false, 0.0f);

	

	UMCOAbilitySystemComponent* TargetMCOASC = Cast<UMCOAbilitySystemComponent>(TargetASC);
	UMCOAbilitySystemComponent* SourceMCOASC = Cast<UMCOAbilitySystemComponent>(SourceASC);
	ISTRUE(nullptr != TargetASC);
	ISTRUE(nullptr != SourceMCOASC);

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
	if (AdditiveStiffness > 0.f)
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
