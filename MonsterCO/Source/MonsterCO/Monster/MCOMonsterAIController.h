#pragma once

#include "MonsterCO.h"
#include "AIController.h"
#include "MCOMonsterAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;


UCLASS()
class MONSTERCO_API AMCOMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMCOMonsterAIController(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

	void RunAI();
	void StopAI();
	void ContinueAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};
