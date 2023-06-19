#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility_CommonAttack.h"
#include "MCOGameplayAbility_ComboAttack.generated.h"

class UMCOMontageDataCombo;

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_ComboAttack : public UMCOGameplayAbility_CommonAttack
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_ComboAttack();
	
protected:
	virtual void SetDefaultDefinition() override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;
	
// --- Combo
protected:
	void SetComboTimer();
	void DoNextCombo();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataCombo> Data;

protected:
	int32 CurrentCombo;
	bool bIsComboCommandPressed;
	bool bIsDoingCombo; 
	
	FTimerHandle ComboTimerHandle;

};
