#pragma once

#include "MonsterCO.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MCOPlayerState.generated.h"

class UMCOAbilitySystemComponent;
class UMCOAttributeSet;



DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterAttributeDelegate, float /*NewValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCharacterTagDelegate, FGameplayTag /*Tag*/, bool /*Added*/);



USTRUCT(BlueprintType)
struct FAttributeDelegateWrapper
{
	GENERATED_BODY()

public:
	FAttributeDelegateWrapper() {}
	FAttributeDelegateWrapper(const FCharacterAttributeDelegate& InDelegate) : AttributeDelegate(InDelegate) {}

	FCharacterAttributeDelegate AttributeDelegate;
	FDelegateHandle DelegateHandle;
};




UCLASS()
class MONSTERCO_API AMCOPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMCOPlayerState();
	void InitializeAbilitySystem();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

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

	UFUNCTION()
	void HandleEventWithTag(const FGameplayTag& InTag, AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& EffectSpec, float Magnitude) const;
	
	template<class UserClass, typename Func>
	void BindAttributeChangedDelegate(const FName& InName, UserClass* InUserObject, Func ToDoFunc);
	
protected:
	void RegisterAttributeChangedDelegate(const FGameplayAttribute& Attribute);
	void OnAttributeChanged(const FOnAttributeChangeData& Data);

public:
	TMap<FName, FAttributeDelegateWrapper> OnAttributeChangedDelegate;
	
protected:
	UPROPERTY()
	TObjectPtr<const UMCOAttributeSet> AttributeSet;


// --- Tag
protected:
	void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:
	FCharacterTagDelegate OnTagChangedDelegate;
};

template<class UserClass, typename Func>
void AMCOPlayerState::BindAttributeChangedDelegate(const FName& InName, UserClass* InUserObject, Func ToDoFunc)
{
	if (false == OnAttributeChangedDelegate.Contains(InName))
	{
		OnAttributeChangedDelegate.Emplace(InName, FAttributeDelegateWrapper());
	}
	
	OnAttributeChangedDelegate[InName].DelegateHandle = OnAttributeChangedDelegate[InName].AttributeDelegate.AddUObject(
		InUserObject,
		ToDoFunc
	);
}
