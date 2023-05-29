#pragma once

#include "CoreMinimal.h"
#include "Ability_Character/MCOGameplayAbility_CommonAttack.h"
#include "MCOGameplayAbility_MonsterMelee.generated.h"

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_MonsterMelee : public UMCOGameplayAbility_CommonAttack
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_MonsterMelee();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;

	virtual void BeginDamaging_Collision() override;
	virtual void EndDamaging_Collision() override;
	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Montage)
	TMap<EMCOCharacterDirection, TObjectPtr<UMCOCommonMontageData>> Data;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Direction)
	EMCOCharacterDirectionOption DirectionOption;
	
};
