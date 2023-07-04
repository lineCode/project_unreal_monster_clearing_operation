#include "AbilitySystem/CharacterCue/MCOGameplayCue_Damaged.h"
#include "NiagaraComponent.h"


AMCOGameplayCue_Damaged::AMCOGameplayCue_Damaged()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;

	SetRootComponent(NiagaraComponent);
}

void AMCOGameplayCue_Damaged::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	// if (EventType == EGameplayCueEvent::OnActive) MCOLOG_C(MCOAbility, TEXT("Instant Cue : OnActive"))
	// else if (EventType == EGameplayCueEvent::WhileActive) MCOLOG_C(MCOAbility, TEXT("Instant Cue : WhileActive"))
	// else if (EventType == EGameplayCueEvent::Executed) MCOLOG_C(MCOAbility, TEXT("Instant Cue : Executed"))
	// else if (EventType == EGameplayCueEvent::Removed) MCOLOG_C(MCOAbility, TEXT("Instant Cue : Removed"))
	
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
	
	ISTRUE(nullptr != NiagaraComponent);
	ISTRUE(nullptr != NiagaraComponent->GetAsset());

	const FHitResult* InHitResult = Parameters.EffectContext.GetHitResult();
	if (nullptr != InHitResult)
	{
		SetActorLocationAndRotation(InHitResult->ImpactPoint, MyTarget->GetActorRotation());

		MCOLOG_C(MCOAbility, TEXT("[%s] Cue Executed : [%s]"), *GetName(), *InHitResult->ImpactPoint.ToString());
	}
	
	NiagaraComponent->ActivateSystem();
}
