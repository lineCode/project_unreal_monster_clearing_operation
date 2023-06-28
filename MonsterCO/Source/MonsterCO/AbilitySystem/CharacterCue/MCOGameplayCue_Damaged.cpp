#include "AbilitySystem/CharacterCue/MCOGameplayCue_Damaged.h"
#include "NiagaraComponent.h"
#include "Game/MCOGameSingleton.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"
#include "NiagaraEffect/MCONiagaraEffectData.h"


AMCOGameplayCue_Damaged::AMCOGameplayCue_Damaged()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;

	SetRootComponent(NiagaraComponent);
}

void AMCOGameplayCue_Damaged::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	// Only "Executed" is called because this is for an instant effect
	if (EventType == EGameplayCueEvent::OnActive) MCOLOG_C(MCOAbility, TEXT("Instant Cue : OnActive"))
	else if (EventType == EGameplayCueEvent::WhileActive) MCOLOG_C(MCOAbility, TEXT("Instant Cue : WhileActive"))
	else if (EventType == EGameplayCueEvent::Executed) MCOLOG_C(MCOAbility, TEXT("Instant Cue : Executed"))
	else if (EventType == EGameplayCueEvent::Removed) MCOLOG_C(MCOAbility, TEXT("Instant Cue : Removed"))
	
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
	
	ISTRUE(nullptr != NiagaraComponent);
	
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MyTarget);
	ensure(CharacterInterface);

	const UMCODamagedData* DamagedData = CharacterInterface->GetDamagedData(EMCOEffectPolicy::Instant);
	ISTRUE(nullptr != DamagedData);
	
	UNiagaraSystem* DamageNiagara = UMCOGameSingleton::Get().NiagaraEffectData->GetDamagedNiagaraEffect(
		DamagedData->DamagedEffectType, DamagedData->DamagedEffectPolicy, DamagedData->DamagedAmount
	);
	ISTRUE(nullptr != DamageNiagara);
	
	//MCOLOG_C(MCOAbility, TEXT("[%s] Cue Activated : [%s]"), *GetName(), *DamageNiagara->GetName());

	NiagaraComponent->SetAsset(DamageNiagara);

	const IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(MyTarget);
	if (nullptr == PlayerInterface)
	{
		NiagaraComponent->SetWorldLocation(DamagedData->DamagedLocation);
	}

	NiagaraComponent->SetWorldRotation(MyTarget->GetActorRotation());
	NiagaraComponent->ActivateSystem();

	CharacterInterface->RemoveDamagedData(EMCOEffectPolicy::Instant);
}
