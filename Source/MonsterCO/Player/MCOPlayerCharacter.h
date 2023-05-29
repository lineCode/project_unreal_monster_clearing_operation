#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacter.h"
#include "InputActionValue.h"
#include "Interface/MCOHUDInterface.h"
#include "MCOPlayerCharacter.generated.h"

class UMCOPlayerControlData;
class UMCOInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UMCOHUDWidget;

UCLASS()
class MONSTERCO_API AMCOPlayerCharacter : public AMCOCharacter, public IMCOHUDInterface
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
	bool CanJumpAction() const;
	bool CanEquipAction() const;
	bool CanDodgeAction() const;
	bool CanDashAction() const;
	virtual bool CanAttack() const override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void SetSpeed(EMCOCharacterSpeed CharacterSpeed) const;
	virtual void ControlMoving(bool bToStop) override;
	
	FVector GetInputDirection() const;
	bool IsDashForward() const;
	
	bool bGetInput;
	
protected:
	UPROPERTY()
	FVector2D MovementVector;
	
// --- Mode & Weapon 
public:
	EMCOModeType GetModeType() const;
	class UMCOPlayerModeComponent* GetModeComponent() const { return ModeComponent; }
	class AMCOWeapon* GetWeapon();

	UPROPERTY(VisibleAnywhere, Category = "MCO|Weapon")
	TObjectPtr<UMCOPlayerModeComponent> ModeComponent;
	
	virtual void OffAllCollision() override;

// --- Components
protected:
	UPROPERTY(VisibleAnywhere, Category = "MCO|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
  
	UPROPERTY(VisibleAnywhere, Category = "MCO|Camera")
	TObjectPtr<UCameraComponent> Camera;

// --- Widget
public:
	virtual void SetHUD(UMCOHUDWidget* InHUDWidget) override;
	void InitializeHUD();
	virtual void ShowMonsterInfo(AMCOCharacter* InCharacter);

	UPROPERTY()
	uint8 bIsMonsterInfoShowed : 1;

	UPROPERTY()
	TSoftObjectPtr<UMCOHUDWidget> HUDWidget;
};
