#pragma once

#include "MonsterCO.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MCOPlayerState.generated.h"

class UMCOAbilitySystemComponent;
class UMCOAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterAttributeDelegate, float /*NewValue*/);

USTRUCT(BlueprintType)
struct FAttributeDelegateWrapper
{
	GENERATED_BODY()

	FAttributeDelegateWrapper() {}
	FAttributeDelegateWrapper(const FCharacterAttributeDelegate& InDelegate) : AttributeDelegate(InDelegate) {}
	FCharacterAttributeDelegate AttributeDelegate;
};

UCLASS()
class MONSTERCO_API AMCOPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMCOPlayerState();
	void InitializeAbilityDelegates();

protected:
	virtual void BeginPlay() override;

// --- Ability System 
public:
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY()
	TObjectPtr<UMCOAbilitySystemComponent> AbilitySystemComponent;

// --- Attributes
public:
	UFUNCTION(BlueprintCallable, Category = "MCO|Attribute")
	bool IsAlive() const;
    
	UFUNCTION(BlueprintCallable, Category = "MCO|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);

	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);
	virtual void HandleOutOfStiffness(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);
	void HandleEventWithTag(const FGameplayTag& InTag, AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude) const;
	
protected:
	UPROPERTY()
	TObjectPtr<const UMCOAttributeSet> AttributeSet;

// --- On Changed
public:
	TMap<FName, FAttributeDelegateWrapper> OnAttributeChangedDelegate;
	
	template<class UserClass, typename Func>
	void CreateOrUseDelegateAndBind(FName InName, UserClass* InUserObject, Func ToDoFunc);
	
protected:
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data);
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	FDelegateHandle HealthChangeDelegateHandle;
	FDelegateHandle MaxHealthChangeDelegateHandle;
	FDelegateHandle StiffnessChangeDelegateHandle;
	FDelegateHandle MaxStiffnessChangeDelegateHandle;
	FDelegateHandle CharacterLevelChangeDelegateHandle;
};


template<class UserClass, typename Func>
void AMCOPlayerState::CreateOrUseDelegateAndBind(FName InName, UserClass* InUserObject, Func ToDoFunc)
{
	if (false == OnAttributeChangedDelegate.Contains(InName))
	{
		OnAttributeChangedDelegate.Emplace(InName, FAttributeDelegateWrapper());
	}
	
	OnAttributeChangedDelegate[InName].AttributeDelegate.AddUObject(InUserObject, ToDoFunc);
}
