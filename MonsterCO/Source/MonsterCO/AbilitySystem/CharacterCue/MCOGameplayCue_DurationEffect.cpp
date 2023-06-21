#include "AbilitySystem/CharacterCue/MCOGameplayCue_DurationEffect.h"
#include "NiagaraComponent.h"
#include "Interface/MCOCharacterInterface.h"


AMCOGameplayCue_DurationEffect::AMCOGameplayCue_DurationEffect()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;

	SetRootComponent(NiagaraComponent);
}

void AMCOGameplayCue_DurationEffect::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	ISTRUE(nullptr != NiagaraComponent);
	
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MyTarget);
	ensure(CharacterInterface);

	UNiagaraSystem* DurationNiagara = CharacterInterface->GetDamagedData().DurationNiagara;
	ISTRUE(nullptr != DurationNiagara);
	
	MCOLOG_C(MCOAbility, TEXT("[%s] Duration Cue Activated : [%s]"), *GetName(), *DurationNiagara->GetName());
	
	NiagaraComponent->SetAsset(DurationNiagara);
	NiagaraComponent->SetWorldRotation(MyTarget->GetActorRotation());
	NiagaraComponent->ActivateSystem();
}
