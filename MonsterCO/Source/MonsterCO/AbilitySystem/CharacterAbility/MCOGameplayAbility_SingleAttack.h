#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_CommonAttack.h"
#include "MCOGameplayAbility_SingleAttack.generated.h"

class UMCOMontageDataSingle;

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_SingleAttack : public UMCOGameplayAbility_CommonAttack
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_SingleAttack();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataSingle> Data;
};
