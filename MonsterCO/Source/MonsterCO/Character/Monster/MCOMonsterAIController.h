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

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
public:
	void RunAI();
	void StopAI();
	void ContinueAI();

public:
	UObject* GetTarget();

private:
	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};
