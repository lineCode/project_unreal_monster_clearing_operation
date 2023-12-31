#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "MCOGameplayAbility_Damaged.generated.h"

class UMCOMontageDataDirectional;

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Damaged : public UMCOGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_Damaged();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataDirectional> Data;
};
