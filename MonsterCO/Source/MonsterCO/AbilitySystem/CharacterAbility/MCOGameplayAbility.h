#pragma once

#include "MonsterCO.h"
#include "Abilities/GameplayAbility.h"
#include "MCOCommonMontageData.h"
#include "GameplayEffectTypes.h"
#include "MCOGameplayAbility.generated.h"

class UMCOActionDefinition;
class AMCOCharacter;
class AController;
class UMCOAbilitySystemComponent;
class IMCOCharacterInterface;
class UMCOActionFragment_Cooldown;
class UMCOActionFragment_Attribute;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
		
public:
	UMCOGameplayAbility();
	
protected:
	void SetTriggerTag(const FGameplayTag& InTag);

protected:
	// To check character condition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MCO|Ability")
	FGameplayTag AbilityTag;

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	bool SetAndCommitAbility(const bool bIsCanBeCancelled, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	void CancelAllAbility();
	void HandleGameplayEventWithTag(const FGameplayTag& InTag);

protected:
	void StopCharacter(const bool& InStop) const;

	
// --- Setting
public:
	EMCOAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MCO|Ability Activation")
	EMCOAbilityActivationPolicy ActivationPolicy;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint8 bActivateAbilityOnGranted:1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint8 bAutoStopCharacter:1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint8 bAutoActivateChargingStaminaAbility:1;
	
	
// --- Getter
protected:
	ACharacter* GetCharacter() const;
	AController* GetController() const;
	IMCOCharacterInterface* GetMCOCharacterInterface() const;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;

// // --- Effect
// protected:
// 	UPROPERTY()
// 	TSubclassOf<UGameplayEffect> TagEffectClass;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MCO|Fragment")
	UMCOActionDefinition* CurrentDefinition;
	
// --- Cooldown
protected:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	void StartCooldownWidget() const;

protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> CooldownEffectClass;
	
	
// --- Attribute Effect
protected:
	bool CheckCanActivateWithStamina() const;
	void ApplyAttributeEffect(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	
private:
	void StopAttributeEffect() const;
	
protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InstantAttributeEffectClass;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InfiniteAttributeEffectClass;
	
	
// --- Stamina charge
protected:
	void ActivateStaminaChargeAbility();
	
	
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
	virtual void OnTaskInterrupted();
	
	UFUNCTION()
	virtual void OnTaskCompletedWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnTaskCancelledWithEventTag(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	virtual void OnGrantedEventTag(FGameplayTag EventTag, FGameplayEventData EventData);
};
