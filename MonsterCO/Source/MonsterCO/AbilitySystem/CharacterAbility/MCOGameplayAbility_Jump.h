#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "MCOGameplayAbility_Jump.generated.h"

class UMCOActionData;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Jump : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_Jump();

protected:
	virtual void SetDefaultDefinition() override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

// --- Ability end delay
protected:
	void EndAbilityAfterDelay();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|End Delay")
	float AbilityEndDelayTime; 
	
	FTimerHandle AbilityEndDelayTimer;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOActionData> Data;
	
};