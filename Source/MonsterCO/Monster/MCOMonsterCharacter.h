#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacter.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "MCOMonsterCharacter.generated.h"

class UMCOMonsterModeComponent;
class UMCOMonsterAIData;


UCLASS()
class MONSTERCO_API AMCOMonsterCharacter : public AMCOCharacter, public IMCOMonsterAIInterface
{
	GENERATED_BODY()

public:
	AMCOMonsterCharacter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	virtual void SetCharacterData() override;
	
// --- AI
public:
	FAICharacterAITaskFinishedDelegate OnAttackFinished;
	FAICharacterAITaskFinishedDelegate OnTurnFinished;

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIMeleeRange() override;
	virtual float GetAITurnSpeed() override;
	virtual FVector GetAITurnVector() override;
	
	virtual void SetTurnVector(const FVector& InTurnVector) override;
	virtual void SetAIAttackDelegate(const FAICharacterAITaskFinishedDelegate& InOnAttackFinished) override;

	virtual bool CanAttack() override;
	virtual void AttackByAI() override;

protected:
	void ContinueAI();
	void StopAI();
	
protected:
	UPROPERTY()
	FVector TurnVector; 
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UMCOMonsterAIData> MonsterAIData;

// --- Mode
public:
	FORCEINLINE UMCOMonsterModeComponent* GetModeComponent() { return ModeComponent; }

	virtual void OffAllCollision() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UMCOMonsterModeComponent> ModeComponent;
	
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UCapsuleComponent> Body;
};
