#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Montage.generated.h"



UCLASS()
class MONSTERCO_API UMCOActionFragment_Montage : public UMCOActionFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionDefinition> ActionDefinition;

public:
	UMCOActionFragment_Cooldown* GetCooldownFragment() const;
	UMCOActionFragment_Attribute* GetAttributeFragment() const;
	UMCOActionFragment_AttackTiming* GetAttackTimingFragment() const;
	UMCOActionFragment_Collision* GetCollisionFragment() const;
};
