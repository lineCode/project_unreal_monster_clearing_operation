#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CharacterAbility/MCOGameplayAbility.h"
#include "MCOGameplayAbility_Invincible.generated.h"



UCLASS()
class MONSTERCO_API UMCOGameplayAbility_Invincible : public UMCOGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMCOGameplayAbility_Invincible();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
