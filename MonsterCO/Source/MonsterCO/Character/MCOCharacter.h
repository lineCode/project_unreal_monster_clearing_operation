#pragma once

#include "MonsterCO.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/MCOAbilitySet.h"
#include "Interface/MCOAttackedInterface.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOCharacterItemInterface.h"
#include "MCOCharacter.generated.h"


class UMCOActionFragment_AttributeEffect;
class UMCOAbilitySystemComponent;
class UMCOAttributeSet;
class UMCOCharacterData;
class UMCOHpWidget;
class UMCOAttributeWidget;
class UMCOItemData;
class UMCOModeComponent;


DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, const UMCOItemData* /*InItemData*/);

USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()

	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedSignature, const AMCOCharacter*, InCharacter);

UCLASS()
class MONSTERCO_API AMCOCharacter : public ACharacter,
									public IAbilitySystemInterface,
									public IMCOCharacterInterface,
									public IMCOAttackedInterface
{
	GENERATED_BODY()

public:
	AMCOCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	void SetCharacterData();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Character")
	TObjectPtr<UMCOCharacterData> CharacterData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Character")
	FName CharacterName;

public:
	virtual void StopCharacterFromMoving(bool bToStop) override;
	virtual void StopCharacterFromTurning(bool bStopTuring) override;

	void DisableMovement() const;
	void DisableAllCollision();
	void DestroyAllAttachedActors() const;
	virtual FVector GetSocketLocation(const FName& InSocketName) override;
	virtual FTransform GetSocketTransform(const FName& InSocketName) override;

	
// --- Ability
public:
	virtual void InitializeCharacter();
	void UninitializeAbilitySystem();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;

	virtual bool CanActivateAbility(const FGameplayTag& InTag) override;

protected:
	UPROPERTY()
	TWeakObjectPtr<UMCOAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TWeakObjectPtr<const UMCOAttributeSet> AttributeSet;
	
	UPROPERTY()
	FMCOAbilitySet_GrantedHandles AbilitySetHandles;
	
	
// --- Attribute
public:
	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	float GetStiffness() const;
	
	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	float GetMaxStiffness() const;
	
	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	float GetStamina() const;
	
	UFUNCTION(BlueprintCallable, Category = "MCO|Character|Attributes")
	float GetMaxStamina() const;
	
	
// --- Tag
public:
	bool HasTag(const FGameplayTag& InTag) const;
	int32 GetTagCount(const FGameplayTag& InTag) const;

// --- Mode
public:
	FORCEINLINE UMCOModeComponent* GetModeComponent() { return ModeComponent; }
	virtual void TurnOnCollision(const FName& InName) override;
	virtual void TurnOffCollision(const FName& InName) override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "MCO|Mode")
	TObjectPtr<UMCOModeComponent> ModeComponent;
	
// --- Attack
public:
	UFUNCTION()
	void ReceiveDamage(UMCOAbilitySystemComponent* SourceASC, float Damage);

	virtual const FMCODamagedData& GetDamagedData() override  { return CurrentDamagedData; }
	virtual void SetDamagedData(const FMCODamagedData&  InDamagedData) override;

	virtual ACharacter* GetAttackedCharacter() override { return this; }
	virtual float GetCapsuleRadius() const override;
	virtual FCollisionBeginOverlapDelegate& GetCollisionBeginOverlapDelegate() override;
	
protected:
	UPROPERTY()
	FMCODamagedData CurrentDamagedData;
	
	
// --- Die
public:
	virtual void Die() override;
	virtual void FinishDying() override;

public:
	FCharacterDiedSignature OnCharacterDeathStarted;
	FCharacterDiedSignature OnCharacterDeathFinished;

	
// --- Widget
public:
	virtual void InitializeWidget(UMCOHpWidget* InHpWidget, UMCOAttributeWidget* InAttributeWidget) override;
};
