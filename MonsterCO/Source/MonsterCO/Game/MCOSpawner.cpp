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
	if (InState == EMCOGameState::LOBBY)
	{
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		SpawnMonster();
	}
	else if (InState == EMCOGameState::REWARD)
	{
	}
}

void AMCOSpawner::SpawnMonster()
{
	const IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
    ensure(nullptr != GameModeInterface);
	const int32 State = 0; // GameModeInterface->GetStage(); // only one monster is available now...
	
	ensure(State < MonstersData->Monsters.Num());

	if (nullptr != SpawnedMonster)
	{
		SpawnedMonster->SetActorLocation(GetActorLocation());
		SpawnedMonster->InitializeCharacter();
	}
	else
	{
		SpawnedMonster = GetWorld()->SpawnActorDeferred<AMCOMonsterCharacter>(MonstersData->Monsters[State], GetTransform());
		ISTRUE(nullptr != SpawnedMonster);
		SpawnedMonster->FinishSpawning(GetTransform());
		SpawnedMonster->OnCharacterDeathFinished.AddUniqueDynamic(this, &ThisClass::OnMonsterDied);
	}		
}

void AMCOSpawner::OnMonsterDied(const AMCOCharacter* InCharacter)
{
	FindSpawnLocation(InCharacter);	
}

void AMCOSpawner::FindSpawnLocation(const AMCOCharacter* InCharacter)
{
	TArray<FHitResult> TraceResults;
	const FVector StartLocation = InCharacter->GetActorLocation();
	const FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -1000.0f); 

	const FCollisionQueryParams Params(
		SCENE_QUERY_STAT(ItemSpawn),
		false,
		InCharacter
	);
	
	const bool bResult = GetWorld()->LineTraceMultiByObjectType(
		TraceResults,
		StartLocation,
		EndLocation,
		ECC_WorldStatic,
		Params
	);

	ISTRUE(true == bResult);

	for (FHitResult& HiResult : TraceResults)
	{
		if (HiResult.GetActor()->IsA(APawn::StaticClass()))
		{
			continue;
		}
		
		PickRandomItem(HiResult.Location);
		break;
	}
}

void AMCOSpawner::PickRandomItem(const FVector& InSpawnLocation)
{
	const UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(ITEMDATA_NAME, Assets);
	ensure(0 < Assets.Num());
	
	const int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);

	const FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (true == AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	if (true == SpawnedItems.Contains(RandomIndex))
	{
		RespawnItem(RandomIndex, InSpawnLocation);
	}
	else
	{
		SpawnNewItem(RandomIndex, InSpawnLocation, AssetPtr);
	}
}

void AMCOSpawner::RespawnItem(const int32& RandomIndex, const FVector& InSpawnLocation)
{
	MCOPRINT(TEXT("Item: %d -> Respawn !"), RandomIndex);
	
	SpawnedItems[RandomIndex]->InitializeItem(InSpawnLocation);
}

void AMCOSpawner::SpawnNewItem(const int32& RandomIndex, const FVector& InSpawnLocation, const FSoftObjectPtr& AssetPtr)
{
	MCOPRINT(TEXT("Item: %d -> New spawn !"), RandomIndex);
		
	FTransform Transform;
	Transform.SetLocation(InSpawnLocation);
	
	AActor* ItemActor = GetWorld()->SpawnActorDeferred<AMCOItem>(ItemClass, Transform);
		
	AMCOItem* Item = Cast<AMCOItem>(ItemActor);
	if (nullptr != Item)
	{
		Item->OnItemDestroyed.AddUObject(this, &ThisClass::OnItemDestroyed);
		SpawnedItems.Add(RandomIndex, Item);
		
		Item->SetData(Cast<UMCOItemData>(AssetPtr.Get()));
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
