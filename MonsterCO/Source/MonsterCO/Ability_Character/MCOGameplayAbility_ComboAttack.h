#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility_CommonAttack.h"
#include "MCOGameplayAbility_ComboAttack.generated.h"

class UMCOComboAttackData;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_ComboAttack : public UMCOGameplayAbility_CommonAttack
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_ComboAttack();
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;

	virtual void BeginDamaging_Collision() override;
	virtual void EndDamaging_Collision() override;
	
// --- Combo
protected:
	void SetComboTimer();
	void DoNextCombo();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|AttackData")
	TObjectPtr<UMCOComboAttackData> Data;

	int32 CurrentCombo;
	bool bIsComboCommandPressed;
	bool bIsDoingCombo; 
	
	FTimerHandle ComboTimerHandle;

};
