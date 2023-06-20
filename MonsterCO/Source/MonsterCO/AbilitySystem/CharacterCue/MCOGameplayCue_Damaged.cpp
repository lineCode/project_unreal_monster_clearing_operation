#include "AbilitySystem/CharacterCue/MCOGameplayCue_Damaged.h"
#include "NiagaraComponent.h"
#include "Interface/MCOCharacterInterface.h"


AMCOGameplayCue_Damaged::AMCOGameplayCue_Damaged()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;

	SetRootComponent(NiagaraComponent);
}

void AMCOGameplayCue_Damaged::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	ISTRUE(nullptr != NiagaraComponent);
	
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MyTarget);
	ensure(CharacterInterface);

	const FVector DamagedLocation = CharacterInterface->GetDamagedData().DamagedLocation;
	UNiagaraSystem* DamageNiagara = CharacterInterface->GetDamagedData().DamagedNiagara;
	const bool bHasDuration = CharacterInterface->GetDamagedData().bHasDuration;

	CharacterInterface->SetDamagedData(FMCODamagedData());
	
	ISTRUE(nullptr != DamageNiagara);
	
	MCOLOG_C(MCOAbility, TEXT("[%s] Cue Activated: Location [%s]"), *GetName(), *DamagedLocation.ToString());
	
	NiagaraComponent->SetAsset(DamageNiagara);

	if (false == bHasDuration)
	{
		NiagaraComponent->SetWorldLocation(DamagedLocation);
	}
	
	NiagaraComponent->SetWorldRotation(MyTarget->GetActorRotation());
	NiagaraComponent->ActivateSystem();
}
