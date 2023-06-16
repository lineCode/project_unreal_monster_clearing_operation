#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "MCOSpawner.generated.h"

class UBoxComponent;
class UMCOMonstersData;
class AMCOMonsterCharacter;
class AMCOCharacter;
class AMCOItem;

UCLASS()
class MONSTERCO_API AMCOSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOSpawner();
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Box;
	
// --- Monster
public:
	void SpawnMonster();

	UFUNCTION()
	void OnMonsterDied(const AMCOCharacter* InCharacter);
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMCOMonstersData> MonstersData;

	UPROPERTY()
	TWeakObjectPtr<AMCOMonsterCharacter> SpawnedMonster;
	
// --- Item
public:
	void FindSpawnLocation(const AMCOCharacter* InCharacter);
	void PickRandomItem(const FVector& InSpawnLocation);
	void RespawnItem(const int32& RandomIndex, const FVector& InSpawnLocation);
	void SpawnNewItem(const int32& RandomIndex, const FVector& InSpawnLocation, const FSoftObjectPtr& AssetPtr);

	UFUNCTION()
	void OnItemDestroyed();

protected:
	UPROPERTY(VisibleAnywhere, Category = Item, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMCOItem> ItemClass;

	UPROPERTY(VisibleAnywhere, Category = Item, Meta = (AllowPrivateAccess = "true"))
	TMap<int32, TWeakObjectPtr<AMCOItem>> SpawnedItems;
};
