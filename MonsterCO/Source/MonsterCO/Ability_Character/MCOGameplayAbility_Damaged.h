#pragma once

#include "MonsterCO.h"
#include "Ability_Character/MCOGameplayAbility.h"
#include "MCOGameplayAbility_Damaged.generated.h"

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Damaged : public UMCOGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_Damaged();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;
	
protected:
// --- Using Montage
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Montage)
	TMap<EMCOCharacterDirection, TObjectPtr<UAnimMontage>> Montages;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Montage)
	EMCOCharacterDirectionOption DirectionOption;
};
