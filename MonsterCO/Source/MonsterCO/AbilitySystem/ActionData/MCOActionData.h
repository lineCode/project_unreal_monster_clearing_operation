#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOActionData.generated.h"

class UMCOActionDefinition;
class UMCOActionFragment_Cooldown;
class UMCOActionFragment_AttributeEffect;
class UMCOActionFragment_AttackTiming;
class UMCOActionFragment_Collision;


UCLASS()
class MONSTERCO_API UMCOActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

// public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
// 	FGameplayTag ActivationTag;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Definition, Instanced)
	TObjectPtr<UMCOActionDefinition> ActionDefinition;

public:
	void UpdateDefinition(UMCOActionDefinition* OutDefinition) const;
	
protected:
	UMCOActionFragment_Cooldown* GetCooldownFragment() const;
	UMCOActionFragment_AttributeEffect* GetAttributeFragment() const;
	UMCOActionFragment_AttackTiming* GetAttackTimingFragment() const;
	UMCOActionFragment_Collision* GetCollisionFragment() const;
};
