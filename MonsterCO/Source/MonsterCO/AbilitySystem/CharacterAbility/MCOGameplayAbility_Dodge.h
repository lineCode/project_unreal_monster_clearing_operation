#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "MCOGameplayAbility_Dodge.generated.h"

class UMCOActionData;

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Dodge : public UMCOGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_Dodge(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTaskFinished();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOActionData> Data;
	
	UPROPERTY(EditAnywhere, Category = MCO)
	float Strength;
	
	UPROPERTY(EditAnywhere, Category = MCO)
	float Duration;

private:
	bool bIsDodgeForward;
};
