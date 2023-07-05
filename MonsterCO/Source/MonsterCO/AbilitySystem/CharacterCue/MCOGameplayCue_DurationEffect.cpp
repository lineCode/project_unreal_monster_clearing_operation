#include "AbilitySystem/CharacterCue/MCOGameplayCue_DurationEffect.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"


AMCOGameplayCue_DurationEffect::AMCOGameplayCue_DurationEffect()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("NAME_Scene"));
	SetRootComponent(Scene);
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;
	NiagaraComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NAME_Audio"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AMCOGameplayCue_DurationEffect::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
	
	ISTRUE(nullptr != NiagaraComponent);
	ISTRUE(nullptr != NiagaraComponent->GetAsset());

	// only WhileActive is called once if this is duration effect 
	if (EventType == EGameplayCueEvent::WhileActive)
	{
		// Get data once if this is duration effect
		// because hit result data will have the same position even though character's moving...
		const FHitResult* InHitResult = Parameters.EffectContext.GetHitResult();
		if (nullptr != InHitResult)
		{
			// Set 'Location Position' of Niagara effect 'ON' !!!
			SetActorLocationAndRotation(InHitResult->ImpactPoint, MyTarget->GetActorRotation());

			//MCOLOG_C(MCOAbility, TEXT("[%s] Duration Cue Executed : [%s]"), *GetName(), *InHitResult->ImpactPoint.ToString());
		}
	}
	else if (EventType == EGameplayCueEvent::Executed)
	{
		NiagaraComponent->ActivateSystem();
	}
}
