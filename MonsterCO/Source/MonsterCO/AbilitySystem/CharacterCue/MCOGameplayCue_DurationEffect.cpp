#include "AbilitySystem/CharacterCue/MCOGameplayCue_DurationEffect.h"
#include "NiagaraComponent.h"
#include "Game/MCOGameSingleton.h"
#include "Interface/MCOCharacterInterface.h"
#include "NiagaraEffect/MCONiagaraEffectData.h"


AMCOGameplayCue_DurationEffect::AMCOGameplayCue_DurationEffect()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->bAutoActivate = false;

	SetRootComponent(NiagaraComponent);
}

void AMCOGameplayCue_DurationEffect::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	// if (EventType == EGameplayCueEvent::OnActive) MCOLOG_C(MCOAbility, TEXT("Duration Cue : OnActive"))
	// else if (EventType == EGameplayCueEvent::WhileActive) MCOLOG_C(MCOAbility, TEXT("Duration Cue : WhileActive"))
	// else if (EventType == EGameplayCueEvent::Executed) MCOLOG_C(MCOAbility, TEXT("Duration Cue : Executed"))
	// else if (EventType == EGameplayCueEvent::Removed) MCOLOG_C(MCOAbility, TEXT("Duration Cue : Removed"))
	
	// Super::HandleGameplayCue(MyTarget, EventType, Parameters);
	//
	// ISTRUE(nullptr != NiagaraComponent);
	//
	// IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MyTarget);
	// ensure(CharacterInterface);
	//
	// // Get data then remove
	// if (EventType == EGameplayCueEvent::WhileActive)
	// {
	// 	const UMCODamagedData* DamagedData = CharacterInterface->GetDamagedData(EMCOEffectPolicy::Duration);
	// 	ISTRUE(nullptr != DamagedData);
	//
	// 	UNiagaraSystem* DamageNiagara = UMCOGameSingleton::Get().NiagaraEffectData->GetDamagedNiagaraEffect(
	// 		DamagedData->DamagedEffectType, DamagedData->DamagedEffectPolicy, DamagedData->DamagedAmount
	// 	);
	// 	ISTRUE(nullptr != DamageNiagara);
	//
	// 	//MCOLOG_C(MCOAbility, TEXT("[%s] Duration Cue Executed : [%s]"), *GetName(), *DamageNiagara->GetName());
	//
	// 	SetActorRotation(MyTarget->GetActorRotation());
	// 	
	// 	NiagaraComponent->SetAsset(DamageNiagara);
	// 	
	// 	CharacterInterface->RemoveDamagedData(EMCOEffectPolicy::Duration);
	// }
	// else if (EventType == EGameplayCueEvent::Executed)
	// {
	// 	NiagaraComponent->ActivateSystem();
	// }
}
