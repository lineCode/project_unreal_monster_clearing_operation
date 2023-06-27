#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CharacterAbility/AttackAbility/MCOGA_CommonAttack.h"
#include "MCOGA_InstantAttack.generated.h"

class UMCOActionData;

UCLASS()
class MONSTERCO_API UMCOGA_InstantAttack : public UMCOGA_CommonAttack
{
	GENERATED_BODY()
	
public:
	UMCOGA_InstantAttack();
	
protected:
	virtual void SetDefaultDefinition() override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOActionData> Data;
};
