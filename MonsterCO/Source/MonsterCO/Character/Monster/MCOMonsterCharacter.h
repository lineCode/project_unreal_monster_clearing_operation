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
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UCapsuleComponent> Body;

	
// --- Ability
public:
	virtual bool CanActivateAbility(const FGameplayTag& InTag) override;
	
// --- AI
protected:
	virtual UObject* GetTarget() override;
	
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIMeleeRange() override;
	virtual float GetAITurnSpeed() override;
	virtual FVector GetAITurnVector() override;
	
	virtual bool IsTurning() override;
	virtual void SetTurnVector(const bool InIsTurning, const FVector& InTurnVector = FVector()) override;
	virtual void SetAIAttackDelegate(const FAICharacterAITaskFinishedDelegate& InOnAttackFinished) override;

	virtual void AttackByAI() override;
	virtual void StopCharacter(bool bToStop) override;

protected:
	void ContinueAI();
	void StopAI();
	
protected:
	UPROPERTY()
	FVector TurnVector;

	bool bIsTurning;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = MCO)
	TObjectPtr<UMCOMonsterAIData> MonsterAIData;
	
};
