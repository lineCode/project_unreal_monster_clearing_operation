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
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;

protected:
	UPROPERTY()
	uint8 bIsToEquip:1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UAnimMontage> MontageOnEquip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UAnimMontage> MontageOnUnequip;
};
