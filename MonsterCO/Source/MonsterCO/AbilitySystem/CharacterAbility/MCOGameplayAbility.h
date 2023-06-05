#pragma once

#include "MonsterCO.h"
#include "Abilities/GameplayAbility.h"
#include "MCOCommonMontageData.h"
#include "GameplayEffectTypes.h"
#include "MCOGameplayAbility.generated.h"

class AMCOCharacter;
class AController;
class UMCOAbilitySystemComponent;
class UMCOActionFragment_Cooldown;
class UMCOActionFragment_Stamina;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
		
public:
	UMCOGameplayAbility();
	
protected:
	void SetID(const EMCOAbilityID& InAbilityID, const FGameplayTag& InActivationTag);
	void SetTriggerTag(const FGameplayTag& InTag);

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	bool SetAndCommitAbility(const bool bIsCanBeCancelled, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	void CancelAllAbility();
	
// --- Getter
protected:
	ACharacter* GetCharacter() const;
	AController* GetController() const;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

// --- Effect
protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> TagEffectClass;
	
// --- Cooldown
protected:
	void UpdateCooldownFragment(const UMCOActionFragment_Cooldown* InCooldownFragment);
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	void StartCooldownWidget() const;

protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> CooldownEffectClass;

private:
	UPROPERTY()
	TObjectPtr<const UMCOActionFragment_Cooldown> CooldownFragment;

// --- Stamina
protected:
	void UpdateStaminaFragment(const UMCOActionFragment_Stamina* InStaminaFragment);
	void ApplyStaminaEffectByPolicy();
	void ActivateStaminaChargeAbility();
	void HandleStaminaChargeEvent();
	
private:
	void ApplyStaminaEffect(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	void StopStaminaEffect();
	
protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InstantStaminaEffectClass;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InfiniteStaminaEffectClass;
	
private:
	UPROPERTY()
	TObjectPtr<const UMCOActionFragment_Stamina> StaminaFragment;

	FTimerHandle StaminaChargingDelayTimer;
	
	
// --- Setting
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Ability")
	FGameplayTag AbilityTag;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Ability")
	EMCOAbilityID AbilityInputID = EMCOAbilityID::None;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Ability")
	EMCOAbilityID AbilityLevelID = EMCOAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability")
	uint8 bActivateAbilityOnGranted:1;

public:
	EMCOAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MCO|Ability Activation")
	EMCOAbilityActivationPolicy ActivationPolicy;

	
// --- Montage Task
protected:
	void StartActivationWithMontage(UAnimMontage* InMontage, const FName& SectionName = NAME_None);
	void StartActivationWithMontageAndEventTag(UAnimMontage* InMontage, const FName& SectionName = NAME_None);
	void JumpToMontageSection(const UAnimMontage* InMontage, const FName& SectionName) const;
	
	UFUNCTION()
	virtual void OnTaskCompleted();

	UFUNCTION()
	virtual void OnTaskCancelled();
	
	UFUNCTION()
	virtual void OnTaskCompletedWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnTaskCancelledWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	virtual void OnGrantedEventTag(FGameplayTag EventTag, FGameplayEventData EventData);
};
