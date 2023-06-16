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
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOActionData> Data;
};
