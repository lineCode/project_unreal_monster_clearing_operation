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
protected:
	virtual bool CanActivateAbility(const FGameplayTag& InTag) override;
	
// --- AI
protected:
	FMCOAICharacterTaskFinishedDelegate OnAttackFinishedDelegate;
	FMCOAICharacterTaskFinishedDelegate OnTurnFinishedDelegate;
	
protected:
	virtual UObject* GetTarget() override;
	
// --- AI/Data
protected:
	virtual float GetAIPatrolRadius() const override;
	virtual float GetAIDetectRange() const override;
	virtual float GetAIAttackRangeMin(const FGameplayTag& InTag) const override;
	virtual float GetAIAttackRangeMax(const FGameplayTag& InTag) const override;
	virtual float GetAITurnSpeed() const override;
	
// --- AI/State
protected:
	virtual FVector GetAITurnVector() const override;
	virtual bool IsTurning() const override;
	virtual void SetTurnVector(const bool InIsTurning, const FVector& InTurnVector = FVector()) override;
	virtual void SetActionDelegate(const FMCOAICharacterTaskFinishedDelegate& InOnAttackFinished) override;
	virtual void OnActionFinished(const EBTNodeResult::Type& InResult) const override;
	
// --- AI/Action
protected:
	virtual void Attack(const FGameplayTag& InTag) const override;

protected:
	void ContinueAI() const;
	void StopAI() const;
	
protected:
	UPROPERTY()
	FVector TurnVector;

	bool bIsTurning;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UMCOMonsterAIData> MonsterAIData;
	
};
