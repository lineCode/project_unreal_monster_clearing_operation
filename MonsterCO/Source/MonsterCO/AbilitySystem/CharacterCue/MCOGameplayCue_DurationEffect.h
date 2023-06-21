#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "MCOGameplayCue_DurationEffect.generated.h"


class UNiagaraComponent;




UCLASS()
class MONSTERCO_API AMCOGameplayCue_DurationEffect : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AMCOGameplayCue_DurationEffect();
	
protected:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComponent;
};
