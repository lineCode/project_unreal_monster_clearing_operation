#pragma once

#include "MonsterCO.h"
#include "Abilities/GameplayAbility.h"
#include "MCOGameplayAbility.generated.h"

class AMCOCharacter;
class AController;
class UMCOAbilitySystemComponent;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
		
public:
	UMCOGameplayAbility();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	bool SetAndCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	void CancelAllAbility();

// --- Getter
protected:
	ACharacter* GetCharacter() const;
	AMCOCharacter* GetMCOCharacter() const;
	AController* GetController() const;
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;

// --- Effect
protected:
	UPROPERTY(EditAnywhere, Category = Effect)
	TSubclassOf<UGameplayEffect> TagEffectClass;
	
// --- Cooldown
protected:
	void SetCooldownGameplayEffect(const float& InDuration, const FGameplayTagContainer& InTags);
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	
	UPROPERTY(EditAnywhere, Category = Cooldown)
	TSubclassOf<UGameplayEffect> CooldownEffectClass;
	
	UPROPERTY(EditAnywhere, Category = Cooldown)
	float CooldownTimeMax;
	
	UPROPERTY(EditAnywhere, Category = Cooldown)
	FGameplayTagContainer CooldownTags;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability")
	uint8 bApplyCooldown : 1;
	
// --- Setting
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Ability")
	FGameplayTag AbilityTag;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Ability")
	EMCOAbilityID AbilityInputID = EMCOAbilityID::None;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Ability")
	EMCOAbilityID AbilityLevelID = EMCOAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability")
	uint8 bActivateAbilityOnGranted:1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Ability")
	uint8 bCanBeCancelled : 1;

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
	
// --- Direction
protected:
	EMCOCharacterDirection GetDirectionFromDegree(const EMCOCharacterDirectionOption InDirectionOption, const float InDegree);
};
