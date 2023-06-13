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


class UMCOActionFragment_Attribute;
class UMCOAbilitySystemComponent;
class UMCOAttributeSet;
class UMCOCharacterData;
class UMCOHpWidget;
class UMCOAttributeWidget;
class UMCOItemData;


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
	public IMCOAttackedInterface,
	public IMCOCharacterItemInterface
{
	GENERATED_BODY()

public:
	AMCOCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Character")
	FName CharacterName;

	void SetCharacterData();
	
// --- Ability
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;

protected:
	virtual void Initialize();

protected:
	UPROPERTY()
	TWeakObjectPtr<UMCOAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TWeakObjectPtr<const UMCOAttributeSet> AttributeSet;
	
	UPROPERTY()
	FMCOAbilitySet_GrantedHandles AbilitySetHandles;
	
public:
	virtual float GetCurrentStamina() const override;
	virtual bool CanChargeStamina() const override;
	virtual bool CanAttack() const override { return true; }
	virtual FVector GetSocketLocation(const FName& InSocketName);

	
// --- Attribute
public:
	UFUNCTION(BlueprintCallable, Category = "MCO|Character")
	virtual int32 GetAbilityLevel(EMCOAbilityID InAbilityLevelID) const;

	UFUNCTION(BlueprintCallable, Category = "MCO|Character")
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
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MCO|Character|Attributes")
	TObjectPtr<UMCOCharacterData> CharacterData;
	
// --- Tag
public:
	bool HasTag(const FGameplayTag& InTag) const;
	int32 GetTagCount(const FGameplayTag& InTag) const;

// --- Damage
public:
	UFUNCTION()
	void ReceiveDamage(UMCOAbilitySystemComponent* SourceASC, float Damage);

	virtual const FMCODamagedData GetDamagedData() override  { return CurrentDamagedData; }
	virtual void SetDamagedData(const FMCODamagedData&  InDamagedData) override;

	virtual void OffAllCollision() override;

	virtual ACharacter* GetAttackedCharacter() override { return this; }
	virtual float GetCapsuleRadius() const override;
	
protected:
	UPROPERTY()
	FMCODamagedData CurrentDamagedData;
	
// --- Die
public:
	virtual void Die() override;

	UFUNCTION(BlueprintCallable, Category = "MCO|Character")
	virtual void FinishDying() override;

	void DestroyAllAttachedActors();

	FCharacterDiedSignature OnCharacterDeathFinished;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MCO|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

// --- Stop
public:
	virtual void StopCharacter(bool bToStop) override;
	
// --- Item
public:
	virtual void TakeItem(const UMCOItemData* InItemData) override;
	virtual void DrinkPotion(const UMCOItemData* InItemData);
	virtual void EquipWeapon(const UMCOItemData* InItemData);
	virtual void ReadScroll(const UMCOItemData* InItemData);
	virtual UMCOActionFragment_Attribute* GetItemAttributeFragment() override;
	virtual void EndTakeItem() override;
	
	UPROPERTY()
	TMap<EMCOItemType, FTakeItemDelegateWrapper> TakeItemActions;
	
	UPROPERTY()
	TObjectPtr<UMCOActionFragment_Attribute> ItemAttributeFragment;
	
// --- Widget
public:
	virtual void InitializeWidget(UMCOHpWidget* InHpWidget, UMCOAttributeWidget* InAttributeWidget) override;
};
