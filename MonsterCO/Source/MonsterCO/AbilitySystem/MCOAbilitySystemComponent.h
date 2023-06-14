#pragma once

#include "MonsterCO.h"
#include "AbilitySystemComponent.h"
#include "MCOAbilitySystemComponent.generated.h"

class UMCOAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceivedDamageDelegate, UMCOAbilitySystemComponent*, SourceASC, float, Damage);


UCLASS()
class MONSTERCO_API UMCOAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMCOAbilitySystemComponent();

	bool bCharacterAbilitySetGiven;

	UPROPERTY()
	TObjectPtr<const UMCOAttributeSet> AttributeSet;

public:
	void TryActivateAbilityByTag(const FGameplayTag& InTag);
	void CancelAbilityByTag(const FGameplayTag& InTag);
	
// --- Input
public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
// --- Damage
public:
	FReceivedDamageDelegate OnDamagedReceived;
	
	virtual void ReceiveDamage(UMCOAbilitySystemComponent* SourceASC, float Damage, float TotalStiffness);
};
