#include "MCODamageExecution.h"
#include "AbilitySystemComponent.h"
#include "MCOAttributeSet.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOCharacterTags.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition DamageDef;
	FGameplayEffectAttributeCaptureDefinition StiffnessDef;

	FDamageStatics()
	{
		// Capture optional Damage set on the damage GE as a Calculation Modifier under the ExecutionCalculation
		DamageDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
		StiffnessDef = FGameplayEffectAttributeCaptureDefinition(UMCOAttributeSet::GetStiffnessAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UMCODamageExecution::UMCODamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().StiffnessDef);
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

	// Source Damage
	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(
		Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_DamageTag, false, -1.0f),
		0.0f
	);
	
	float Stiffness = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StiffnessDef, EvaluationParameters, Stiffness);
	Stiffness = FMath::Max<float>(
		Spec.GetSetByCallerMagnitude(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag, false, -1.0f),
		0.0f
	);

	UMCOAbilitySystemComponent* TargetMCOASC = Cast<UMCOAbilitySystemComponent>(TargetASC);
	UMCOAbilitySystemComponent* SourceMCOASC = Cast<UMCOAbilitySystemComponent>(SourceASC);
	ISTRUE(nullptr != TargetASC);
	ISTRUE(nullptr != SourceMCOASC);

	// Check Dodge
	if (true == TargetASC->HasMatchingGameplayTag(FMCOCharacterTags::Get().DodgeTag))
	{
		MCOPRINT(TEXT("Escaped attack by dodging"));
		return;
	}
	
	// Broadcast damages to Target ASC
	TargetMCOASC->ReceiveDamage(SourceMCOASC, Damage, TargetMCOASC->AttributeSet->GetStiffness() + Stiffness);
	
	if (Damage > 0.f)
	{
		// Set the Target's Damage meta attribute
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UMCOAttributeSet::GetDamageAttribute(), 
				EGameplayModOp::Additive, 
				Damage
			)
		);
	}
	if (Stiffness > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UMCOAttributeSet::GetStiffnessAttribute(), 
				EGameplayModOp::Additive, 
				Stiffness
			)
		);
	}
}
