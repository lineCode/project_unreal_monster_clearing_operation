#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "MCOGameplayAbility_Equip.generated.h"


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Equip : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_Equip();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void OnGrantedEventTag(FGameplayTag EventTag, FGameplayEventData EventData) override;
	
protected:
	UPROPERTY()
	uint32 bIsToEquip:1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UAnimMontage> MontageOnEquip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UAnimMontage> MontageOnUnequip;
};
