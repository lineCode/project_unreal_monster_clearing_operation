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
class UWidgetComponent;



DECLARE_DELEGATE_OneParam(FOnStaminaStoppedDelegate, const float& /*InCurrentValue*/);
DECLARE_DELEGATE_OneParam(FOnStaminaChangedDelegate, const float& /*InPercent*/);
DECLARE_MULTICAST_DELEGATE(FOnMonsterFirstHit);



UCLASS()
class MONSTERCO_API AMCOPlayerCharacter : public AMCOCharacter, public IMCOHUDInterface, public IMCOPlayerInterface
{
	GENERATED_BODY()

public:
	AMCOPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	
// --- Ability System
protected:
	virtual void OnRep_PlayerState() override;
	virtual void Initialize() override;
	
// -- Control Data
protected:
	void SetControlData();
	
	UPROPERTY(EditAnywhere, Category = "MCO|Control")
	TObjectPtr<UMCOPlayerControlData> CharacterControlData;
	
// --- Input
public:	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	UPROPERTY(EditAnywhere, Category = "MCO|Input")
	TObjectPtr<UMCOInputConfig> InputConfig;

	
// --- Action
public:
	bool CheckCanMoveWithTags() const;
	bool CanMoveCamera() const;
	bool CanMoveCharacter() const;
	virtual bool CanJumpAction() const override;
	virtual bool CanEquipAction() const override;
	virtual bool CanDodgeAction() const override;
	virtual bool CanDashAction() const override;
	virtual bool CanAttack() const override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	virtual void SetSpeed(const EMCOCharacterSpeed& CharacterSpeed) const override; 
	virtual void StopCharacter(bool bToStop) override;
	
	virtual FVector GetInputWorldDirection() const override;
	virtual bool IsInputForward() const override;
	
	bool bGetInput;
	
// --- Mode & Weapon 
public:
	EMCOModeType GetModeType() const;
	
	virtual void OffAllCollision() override;
	virtual bool IsEquipped() override;
	virtual void SetEquippedWithoutAnimation() override;
	virtual void SwitchEquipUnequip() override;
	virtual void BeginAnimation_Equip() override;
	virtual void EndAnimation_Equip() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "MCO|Weapon")
	TObjectPtr<UMCOPlayerModeComponent> ModeComponent;

// --- Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "MCO|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
  
	UPROPERTY(VisibleAnywhere, Category = "MCO|Camera")
	TObjectPtr<UCameraComponent> Camera;

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
