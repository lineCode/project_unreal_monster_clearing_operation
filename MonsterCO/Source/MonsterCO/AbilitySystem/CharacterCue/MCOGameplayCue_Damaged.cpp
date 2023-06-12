#include "AbilitySystem/CharacterCue/MCOGameplayCue_Damaged.h"
#include "NiagaraComponent.h"
#include "Interface/MCOCharacterInterface.h"


AMCOGameplayCue_Damaged::AMCOGameplayCue_Damaged()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;
}

void AMCOGameplayCue_Damaged::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	MCOPRINT(TEXT("%s"), *MyTarget->GetName());

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MyTarget);
	ensure(CharacterInterface);

	const FVector DamagedLocation = CharacterInterface->GetDamagedData().DamagedLocation;
	UNiagaraSystem* DamageNiagara = CharacterInterface->GetDamagedData().DamagedNiagara;
	
	NiagaraComponent->SetAsset(DamageNiagara);
	NiagaraComponent->SetWorldLocation(DamagedLocation);
	NiagaraComponent->SetWorldRotation(MyTarget->GetActorRotation());
	NiagaraComponent->ActivateSystem();	
}
