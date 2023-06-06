#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CharacterAbility/MCOGameplayAbility.h"
#include "MCOGameplayAbility_ChargeStamina.generated.h"

class UMCOActionData;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_ChargeStamina : public UMCOGameplayAbility
{
	GENERATED_BODY()
public:
	UMCOGameplayAbility_ChargeStamina();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOActionData> Data;
};
