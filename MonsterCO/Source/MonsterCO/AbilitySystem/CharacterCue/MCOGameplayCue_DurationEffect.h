#pragma once

#include "MonsterCO.h"
#include "GameplayCueNotify_Actor.h"
#include "MCOGameplayCue_DurationEffect.generated.h"


class UNiagaraComponent;
class UAudioComponent;



UCLASS()
class MONSTERCO_API AMCOGameplayCue_DurationEffect : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AMCOGameplayCue_DurationEffect();
	
protected:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Scene;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComponent;
};
