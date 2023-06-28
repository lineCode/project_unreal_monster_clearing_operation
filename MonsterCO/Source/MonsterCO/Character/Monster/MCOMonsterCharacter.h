#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacter.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "MCOMonsterCharacter.generated.h"

class UMCOMonsterModeComponent;
class UMCOMonsterAIData;


UCLASS()
class MONSTERCO_API AMCOMonsterCharacter : public AMCOCharacter,
											public IMCOMonsterAIInterface
{
	GENERATED_BODY()

public:
	AMCOMonsterCharacter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

protected:
	virtual void StopCharacterFromMoving(bool bToStop) override;
	virtual void StopCharacterFromTurning(bool bStopTuring) override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UCapsuleComponent> Body;
	
// --- Game State
public:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);
	
// --- Ability
public:
	virtual void InitializeCharacter() override;

protected:
	virtual bool CanActivateAbility(const FGameplayTag& InTag) override;
	
// --- AI
protected:
	FMCOActivateActionDelegate OnActivateActionDelegate;
	FMCOActionFinishedDelegate OnActionFinishedDelegate;
	
protected:
	virtual UObject* GetTarget() override;
	
// --- AI/Data
protected:
	virtual float GetAIPatrolRadius() const override;
	virtual float GetAIDetectRange() const override;
	virtual float GetAIAttackRangeMin(const FGameplayTag& InTag) const override;
	virtual float GetAIAttackRangeMax(const FGameplayTag& InTag) const override;
	virtual float GetAITurnSpeed() const override;
	virtual float GetAIFlySpeed() const override;
	virtual float GetHalfHeight() const override;
	
// --- AI/State
protected:
	virtual void SetMovementMode(EMovementMode InMode) override;
	virtual void SetFlyMode(EMCOMonsterFlyMode InFlyMode) override;
	virtual EMCOMonsterFlyMode GetFlyMode() override;
	virtual void SetGravity(float InGravity) override;
	virtual void AddForce(FVector InForce) override;
	virtual FVector GetVelocity() override;
	virtual void SetVelocity(FVector InVelocity) override;
	virtual bool IsTurning() const override;
	virtual FVector GetTurnVector() const override;
	virtual void SetTurnVector(const FVector& InTurnVector = FVector()) override;

	virtual void SetActivateActionDelegate(const FMCOActivateActionDelegate& InOnActivateAction) override;
	virtual void OnActivateAction() override;
	virtual void SetActionFinishedDelegate(const FMCOActionFinishedDelegate& InOnActionFinished) override;
	virtual void OnActionFinished(const EBTNodeResult::Type& InResult) override;
	virtual void SetDamagedInBlackBoard(bool IsDamaged) const override;
	
// --- AI/Action
protected:
	virtual bool TryActivateAbilityByTag(const FGameplayTag& InTag) const override;
	virtual void CancelAbilityByTag(const FGameplayTag& InTag) const override;

protected:
	void ContinueAI() const;
	void StopAI() const;
	
protected:
	UPROPERTY()
	FVector TurnVector;

	UPROPERTY()
	EMCOMonsterFlyMode FlyMode;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UMCOMonsterAIData> MonsterAIData;

// --- Die
protected:
	virtual void Die() override;
	
};
