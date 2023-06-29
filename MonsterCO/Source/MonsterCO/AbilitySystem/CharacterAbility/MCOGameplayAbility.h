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
class UMCOActionFragment_AttributeEffect;
class UMCOActionFragment_MonsterAI;


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

	// To cancel itself on end ability 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MCO|Ability")
	FGameplayTag EffectTag;

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	virtual void SetDefaultDefinition();
	bool SetAndCommitAbility(bool bIsCanBeCancelled, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	void CancelAllAbility();
	void HandleGameplayEventWithTag(const FGameplayTag& InTag);

protected:
	void StopCharacterFromMoving(bool bStopMoving) const;
	void StopCharacterFromTurning(bool bStopTurning) const;

	
// --- Setting
public:
	EMCOAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MCO|Ability Activation")
	EMCOAbilityActivationPolicy ActivationPolicy;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint32 bActivateAbilityOnGranted:1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint32 bApplyCooldownOnGranted:1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint32 bAutoStopCharacter:1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability Activation")
	uint32 bAutoActivateChargingStaminaAbility:1;
	
	
// --- Getter
protected:
	AActor* GetActor() const;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MCO|Fragment")
	UMCOActionDefinition* CurrentDefinition;

	// This is just for stamina & cooltime
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InstantEffectClass;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DurationEffectClass;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InfiniteEffectClass;
	
// --- Cooldown
protected:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	void StartCooldownWidget() const;

	
// --- Attribute Effect
protected:
	bool CheckCanActivateWithStamina() const;

	// Startup effect for ability (Stamina consumption, item effects...)
	// This will be cancelled when this ability ends
	void ApplyAbilityEffectSelf(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	void CancelAbilityEffectsSelf() const;

protected:
	bool ApplyEffect(UAbilitySystemComponent* ASC, const UMCOActionFragment_AttributeEffect* AttributeFragment, const EMCOEffectPolicy& InPolicy, const TSubclassOf<UGameplayEffect>& EffectClass, const bool& IsAbilityEffect = false) const;

	
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


// --- Monster AI
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "MCO|MonsterAI")
	TObjectPtr<UMCOActionFragment_MonsterAI> MonsterAIFragment;
};
