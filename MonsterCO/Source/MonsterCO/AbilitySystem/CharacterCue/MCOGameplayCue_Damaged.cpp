#include "AbilitySystem/CharacterCue/MCOGameplayCue_Damaged.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"


AMCOGameplayCue_Damaged::AMCOGameplayCue_Damaged()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("NAME_Scene"));
	SetRootComponent(Scene);
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;
	NiagaraComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NAME_Audio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
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
		// Set 'Location Position' of Niagara effect 'ON' !!!
		SetActorLocationAndRotation(InHitResult->ImpactPoint, MyTarget->GetActorRotation());

		//MCOLOG_C(MCOAbility, TEXT("[%s] Instant Cue Executed : [%s]"), *GetName(), *InHitResult->ImpactPoint.ToString());
	}
	
	NiagaraComponent->ActivateSystem();

	ISTRUE(nullptr != AudioComponent);
	ISTRUE(nullptr != AudioComponent->GetSound());
	
	AudioComponent->Stop();
	AudioComponent->Activate();
}
