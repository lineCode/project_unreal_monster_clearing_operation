#pragma once

#include "CoreMinimal.h"
#include "MCOGameplayAbility.h"
#include "MCOGameplayAbility_Death.generated.h"

class UMCOMontageDataDirectional;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Death : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_Death();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void SetDilation();
	void RestoreTimeDilation();
	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	TObjectPtr<UMCOMontageDataDirectional> Data;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	float DilationTime;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = MCO)
	float DilationDuration;

	FTimerHandle TimeDilationHandle;
};
