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
	virtual bool TryActivateAbilityByTag(const FGameplayTag& InTag) const override;
	virtual void CancelAbilityByTag(const FGameplayTag& InTag) const override;
	
// --- AI
protected:
	void RestartAI() const;
	void StopAI() const;
		
	
// --- AI/Data
protected:
	virtual UObject* GetTarget() override;
	virtual float GetAIPatrolRadius() const override;
	virtual float GetAIDetectRange() const override;
	virtual float GetAIAttackRangeMin(const FGameplayTag& InTag) const override;
	virtual float GetAIAttackRangeMax(const FGameplayTag& InTag) const override;
	virtual float GetAITurnSpeed() const override;
	virtual float GetAIFlySpeed() const override;
	virtual float GetHalfHeight() const override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UMCOMonsterAIData> MonsterAIData;
	
// --- AI/Fly
protected:
	virtual void SetMovementMode(EMovementMode InMode) override;
	virtual void SetFlyMode(EMCOMonsterFlyMode InFlyMode) override;
	virtual EMCOMonsterFlyMode GetFlyMode() override;
	virtual void SetGravity(float InGravity) override;
	virtual void AddForce(FVector InForce) override;
	virtual FVector GetVelocity() override;
	virtual void SetVelocity(FVector InVelocity) override;
	
	UPROPERTY()
	EMCOMonsterFlyMode FlyMode;
	
// --- AI/Turn
protected:
	virtual bool IsTurning() const override;
	virtual FVector GetTurnVector() const override;
	virtual void SetTurnVector(FVector InTurnVector) override;

	UPROPERTY()
	FVector TurnVector;
	
// --- AI/Task
protected:
	virtual void SetActivateActionDelegate(const FMCOActivateActionDelegate& InOnActivateAction) override;
	virtual void OnActivateAction() override;
	virtual void SetActionFinishedDelegate(const FMCOActionFinishedDelegate& InOnActionFinished) override;
	virtual void OnActionFinished(EBTNodeResult::Type InResult) override;
	virtual void SetDamagedInBlackBoard(bool IsDamaged) const override;

protected:
	FMCOActivateActionDelegate OnActivateActionDelegate;
	FMCOActionFinishedDelegate OnActionFinishedDelegate;
	

// --- Die
protected:
	virtual void Die() override;
	
};
