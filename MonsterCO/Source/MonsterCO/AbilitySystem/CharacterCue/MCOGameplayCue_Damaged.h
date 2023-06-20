#pragma once

#include "MonsterCO.h"
#include "GameplayCueNotify_Actor.h"
#include "MCOGameplayCue_Damaged.generated.h"


class UNiagaraComponent;



UCLASS()
class MONSTERCO_API AMCOGameplayCue_Damaged : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AMCOGameplayCue_Damaged();
	
protected:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComponent;
};
