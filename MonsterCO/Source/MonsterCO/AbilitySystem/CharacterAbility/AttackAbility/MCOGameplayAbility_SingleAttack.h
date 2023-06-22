#pragma once

#include "CoreMinimal.h"
#include "MCOGA_CommonAttack.h"
#include "MCOGameplayAbility_SingleAttack.generated.h"

class UMCOMontageDataSingle;

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_SingleAttack : public UMCOGA_CommonAttack
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_SingleAttack();
	
protected:
	virtual void SetDefaultDefinition() override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataSingle> Data;
};
