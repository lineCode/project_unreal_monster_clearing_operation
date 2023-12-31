#include "MCOSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "MCOMonstersData.h"
#include "Character/Monster/MCOMonsterCharacter.h"
#include "Item/MCOItem.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Item/MCOItemData.h"


AMCOSpawner::AMCOSpawner()
{
	// Box
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// Monster
	GETASSET(MonstersData, UMCOMonstersData, TEXT("/Game/Data/Monster/DA_MonstersData.DA_MonstersData"))
	
	// Item
	ItemClass = AMCOItem::StaticClass();
}

void AMCOSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
}

void AMCOSpawner::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::TITLE)
	{
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		SpawnMonster();
	}
	else if (InState == EMCOGameState::REWARD)
	{
		FindSpawnLocationThenSpawnItem();
	}
}

void AMCOSpawner::SpawnMonster()
{
	const IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
    ensure(nullptr != GameModeInterface);
	const int32 StateIdx = GameModeInterface->GetStage() - 1; // set 0 if only one monster is available...
	
	ensure(StateIdx < MonstersData->Monsters.Num());

	if (nullptr != SpawnedMonster)
	{
		SpawnedMonster->SetActorLocation(GetActorLocation());
		SpawnedMonster->InitializeCharacter();
	}
	else
	{
		SpawnedMonster = GetWorld()->SpawnActorDeferred<AMCOMonsterCharacter>(MonstersData->Monsters[StateIdx], GetTransform());
		ISTRUE(nullptr != SpawnedMonster);
		SpawnedMonster->FinishSpawning(GetTransform());
		SpawnedMonster->OnCharacterDeathFinished.AddUniqueDynamic(this, &ThisClass::OnMonsterDied);
	}		
}

void AMCOSpawner::OnMonsterDied(const AMCOCharacter* InCharacter)
{
	MonsterDiedLocation = InCharacter->GetActorLocation();
	
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::MONSTER_DIED);
}

void AMCOSpawner::FindSpawnLocationThenSpawnItem()
{
	TArray<FHitResult> TraceResults;
	const FVector StartLocation = MonsterDiedLocation;
	const FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -1000.0f); 

	const FCollisionQueryParams Params(
		SCENE_QUERY_STAT(ItemSpawn),
		false,
		SpawnedMonster.Get()
	);
	
	const bool bResult = GetWorld()->LineTraceMultiByObjectType(
		TraceResults,
		StartLocation,
		EndLocation,
		ECC_WorldStatic,
		Params
	);
	
#if ENABLE_DRAW_DEBUG
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.0f);
#endif
	
	ISTRUE(true == bResult);

	for (FHitResult& HiResult : TraceResults)
	{
		if (HiResult.GetActor()->IsA(APawn::StaticClass()))
		{
			continue;
		}
		
#if ENABLE_DRAW_DEBUG
		//DrawDebugPoint(GetWorld(), HiResult.ImpactPoint, 10.0f, FColor::Blue, false, 2.0f);
#endif
		
		PickRandomItem(HiResult.ImpactPoint);
		break;
	}
}

void AMCOSpawner::PickRandomItem(const FVector& InSpawnLocation)
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> PrimaryAssetIds;
	Manager.GetPrimaryAssetIdList(ITEMDATA_NAME, PrimaryAssetIds);
	ensure(0 < PrimaryAssetIds.Num());
	
	const int32 RandomIndex = FMath::RandRange(0, PrimaryAssetIds.Num() - 1);

	// Respawn if exists
	if (true == SpawnedItems.Contains(RandomIndex))
	{
		RespawnItem(RandomIndex, InSpawnLocation);
		return;
	}

	// New spawn 
	const FPrimaryAssetId PrimaryAssetId = PrimaryAssetIds[RandomIndex];

	FStreamableDelegate Delegate = FStreamableDelegate::CreateLambda(
	[RandomIndex, PrimaryAssetId, InSpawnLocation, this]()
	{
		SpawnNewItem(RandomIndex, PrimaryAssetId, InSpawnLocation);
	});
    
	TArray<FName> Bundles;
	
	// Async load
	Manager.LoadPrimaryAsset(PrimaryAssetId, Bundles, Delegate);
}

void AMCOSpawner::RespawnItem(const int32& RandomIndex, const FVector& InSpawnLocation)
{
	MCOPRINT(TEXT("Item: %d -> Respawn !"), RandomIndex);
	
	SpawnedItems[RandomIndex]->InitializeItem(InSpawnLocation);
}

void AMCOSpawner::SpawnNewItem(const int32& RandomIndex, const FPrimaryAssetId& AssetId, const FVector& InSpawnLocation)
{
	MCOPRINT(TEXT("Item: %d -> New spawn !"), RandomIndex);

	UAssetManager& Manager = UAssetManager::Get();
	
	FTransform Transform;
	Transform.SetLocation(InSpawnLocation);
	
	AActor* ItemActor = GetWorld()->SpawnActorDeferred<AMCOItem>(ItemClass, Transform);
		
	AMCOItem* Item = Cast<AMCOItem>(ItemActor);
	if (nullptr != Item)
	{
		Item->OnItemDestroyed.AddUObject(this, &ThisClass::OnItemDestroyed);
		SpawnedItems.Add(RandomIndex, Item);

		Item->SetData(Cast<UMCOItemData>(Manager.GetPrimaryAssetObject(AssetId)));
	}

	if (nullptr != ItemActor)
	{
		ItemActor->FinishSpawning(Transform);
	}
}

void AMCOSpawner::OnItemDestroyed()
{
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::NEXT);
}