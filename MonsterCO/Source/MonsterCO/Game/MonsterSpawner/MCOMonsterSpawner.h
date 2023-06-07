#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "MCOMonsterSpawner.generated.h"

class UBoxComponent;
class UMCOMonstersData;
class AMCOMonsterCharacter;
class AMCOCharacter;
class AMCOItem;

UCLASS()
class MONSTERCO_API AMCOMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOMonsterSpawner();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Box;
	
// --- Monster
public:
	void SpawnMonster(const int32& InPhase);

	UFUNCTION()
	void OnMonsterDied(const AMCOCharacter* InCharacter);

	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMCOMonstersData> MonstersData;

	UPROPERTY()
	TWeakObjectPtr<AMCOMonsterCharacter> SpawnedMonster;
	
// --- Item
public:
	void SpawnItem(const FVector& InSpawnLocation);

	UFUNCTION()
	void OnItemDestroyed();

	FTimerHandle Timer;
	void Tester();

protected:
	UPROPERTY(VisibleAnywhere, Category = Item, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMCOItem> ItemClass;

	UPROPERTY(VisibleAnywhere, Category = Item, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<AMCOItem>> Items;
};
