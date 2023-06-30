#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacter.h"
#include "InputActionValue.h"
#include "Interface/MCOHUDInterface.h"
#include "Interface/MCOPlayerInterface.h"
#include "MCOPlayerCharacter.generated.h"

class UMCOPlayerModeComponent;
class AMCOWeapon;
class UMCOPlayerControlData;
class UMCOInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UMCOHUDWidget;
class UMCOOptionWidget;
class UWidgetComponent;
class UMCOPlayerSetting;



DECLARE_DELEGATE_OneParam(FOnStaminaStoppedDelegate, const float& /*InCurrentValue*/);
DECLARE_DELEGATE_OneParam(FOnStaminaChangedDelegate, const float& /*InPercent*/);
DECLARE_MULTICAST_DELEGATE(FOnMonsterFirstHit);



UCLASS()
class MONSTERCO_API AMCOPlayerCharacter : public AMCOCharacter,
											public IMCOHUDInterface,
											public IMCOPlayerInterface,
											public IMCOCharacterItemInterface
{
	GENERATED_BODY()

public:
	AMCOPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	
	
// -- Control Data
protected:
	void SetControlData();

protected:
	UPROPERTY(EditAnywhere, Category = "MCO|Control")
	TObjectPtr<UMCOPlayerControlData> CharacterControlData;

// --- Game State
public:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);
	
// --- Settings
protected:
	UPROPERTY()
	TObjectPtr<UMCOPlayerSetting> PlayerSetting;

	
// --- Ability System
public:
	virtual void InitializeCharacter() override;

	
// --- Input
public:	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(EditAnywhere, Category = "MCO|Input")
	TObjectPtr<UMCOInputConfig> InputConfig;

	UPROPERTY()
	FVector2D InputVector;

		
// --- Tag
protected:
	virtual void OnTagChanged(FGameplayTag InTag, bool IsSet) override;

	
// --- Action
public:
	bool CheckCanMoveWithTags() const;
	bool CanMoveCamera() const;
	bool CanMoveCharacter() const;
	virtual bool CanActivateAbility(const FGameplayTag& InTag) override;

	void Option();
	void UnOption();
	
	void Move(const FInputActionValue& Value);
	void MoveReleased();
	void Look(const FInputActionValue& Value);
	
	virtual void SetSpeed(const EMCOCharacterSpeed& CharacterSpeed) const override; 
	virtual void StopCharacterFromMoving(bool bStopMoving) override;
	virtual void StopCharacterFromTurning(bool bStopTuring) override;
	
	virtual FVector GetInputWorldDirection() const override;
	virtual bool IsInputForward() const override;
	
	bool bCanMoveByInput;
	bool bCanTurnByInput;

	bool bIsOptionOpened;

	
// --- Mode & Weapon 
public:
	virtual EMCOPlayerMode GetModeType() const override;
	virtual bool IsEquipped() override;
	virtual void EquipInstantly() override;
	virtual void SwitchEquipUnequip() override;
	virtual void BeginAnimation_Equip() override;
	virtual void EndAnimation_Equip() override;

	
// --- Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "MCO|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
  
	UPROPERTY(VisibleAnywhere, Category = "MCO|Camera")
	TObjectPtr<UCameraComponent> Camera;

	
// --- Die
public:
	virtual void FinishDying() override;

	
// --- Item
public:
	virtual void TakeItem(const UMCOItemData* InItemData) override;
	virtual void DrinkPotion(const UMCOItemData* InItemData);
	virtual void EquipWeapon(const UMCOItemData* InItemData);
	virtual void ReadScroll(const UMCOItemData* InItemData);
	virtual UMCOActionFragment_AttributeEffect* GetItemAttributeFragment() override;
	virtual void EndTakeItem() override;
	
	UPROPERTY()
	TMap<EMCOItemType, FTakeItemDelegateWrapper> TakeItemActions;
	
	UPROPERTY()
	TObjectPtr<UMCOActionFragment_AttributeEffect> ItemAttributeFragment;

	
// --- Widget
public:
	void InitializeHUD(UMCOHUDWidget* InHUDWidget);
	// virtual void SetHUD(UMCOHUDWidget* InHUDWidget) override;
	// virtual void StartCooldownWidget(const FGameplayTag& InTag, const float& InCooldownTime) const override;

	
// --- Widget/Stamina
public:
	virtual void SetupStaminaWidget(UMCOStaminaWidget* InStaminaWidget) override;
	
protected:
	void OnStaminaChanged(float NewStaminaValue);
	void StartStaminaTimer();
	void StopStaminaTimer();
	void UpdateStaminaWidget();
	
protected:
	FOnStaminaChangedDelegate OnStaminaChangedDelegate;
	FOnStaminaStoppedDelegate OnStaminaStoppedDelegate;

	bool bIsStaminaTimerTicking = false;
	float AdditiveStaminaValueForWidget;
	float CurrentStaminaForWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	FTimerHandle StaminaTimerHandle;

	
// --- Widget/Monster
protected:
	virtual void ShowMonsterInfo(IMCOCharacterInterface* InCharacter) override;

protected:
	FOnMonsterFirstHit OnMonsterFirstHit;
	
	UPROPERTY()
	uint8 bIsMonsterInfoShowed : 1;

	UPROPERTY()
	TWeakObjectPtr<UMCOHUDWidget> HUDWidget;
};
