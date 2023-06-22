#pragma once

#include "CoreMinimal.h"
#include "MCOGA_CommonAttack.h"
#include "MCOGA_MonsterDirectionalAttack.generated.h"

class UMCOMontageDataDirectional;


UCLASS()
class MONSTERCO_API UMCOGA_MonsterDirectionalAttack : public UMCOGA_CommonAttack
{
	GENERATED_BODY()
	
public:
	UMCOGA_MonsterDirectionalAttack();

protected:
	virtual void SetDefaultDefinition() override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataDirectional> Data;
	
};
