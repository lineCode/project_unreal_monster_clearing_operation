#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "MCOMonsterSpawner.generated.h"

class UBoxComponent;
class UMCOMonstersData;
class AMCOMonsterCharacter;


UCLASS()
class MONSTERCO_API AMCOMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOMonsterSpawner();
	virtual void BeginPlay() override;
	
	void SpawnMonster(int32 InPhase);

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMCOMonstersData> MonstersData;

	UPROPERTY()
	TSoftObjectPtr<AMCOMonsterCharacter> SpawnedMonster;
};
