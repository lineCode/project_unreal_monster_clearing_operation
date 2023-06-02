#pragma once

#include "CoreMinimal.h"
#include "MCOGameplayAbility_CommonAttack.h"
#include "MCOGameplayAbility_MonsterMelee.generated.h"

class UMCOMontageDataDirectional;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_MonsterMelee : public UMCOGameplayAbility_CommonAttack
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_MonsterMelee();

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;

	virtual void BeginDamaging_Collision() override;
	virtual void EndDamaging_Collision() override;
	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataDirectional> Data;
	
};
