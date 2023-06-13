#include "MCOMonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "MCOMonstersData.h"
#include "Character/Monster/MCOMonsterCharacter.h"
#include "Interface/MCOGameModeInterface.h"
#include "Item/MCOItem.h"
#include "GameFramework/GameModeBase.h"


AMCOMonsterSpawner::AMCOMonsterSpawner()
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

void AMCOMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	const IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	SpawnMonster(GameModeInterface->GetPhase());
}

void AMCOMonsterSpawner::SpawnMonster(const int32& InStage)
{
	ISTRUE(InStage < MonstersData->Monsters.Num());
	
	SpawnedMonster = GetWorld()->SpawnActorDeferred<AMCOMonsterCharacter>(MonstersData->Monsters[InStage], GetTransform());
	ISTRUE(nullptr != SpawnedMonster);

	SpawnedMonster->OnCharacterDeathFinished.AddUniqueDynamic(this, &ThisClass::OnMonsterDied);
	SpawnedMonster->FinishSpawning(GetTransform());
}

void AMCOMonsterSpawner::OnMonsterDied(const AMCOCharacter* InCharacter)
{
	ensure(nullptr != InCharacter);

	FHitResult TraceResult;
	FVector StartLocation = InCharacter->GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -1000.0f); 

	const FCollisionQueryParams Params;
	
	if (GetWorld()->LineTraceSingleByChannel(
		TraceResult,
		StartLocation,
		EndLocation,
		ECC_WorldStatic,
		Params))
	{
		SpawnItem(TraceResult.Location);
	}
}

void AMCOMonsterSpawner::SpawnItem(const FVector& InSpawnLocation)
{
	FTransform Transform;
	Transform.SetLocation(InSpawnLocation);
	
	AActor* ItemActor = GetWorld()->SpawnActorDeferred<AMCOItem>(ItemClass, Transform);
		
	AMCOItem* Item = Cast<AMCOItem>(ItemActor);
	if (nullptr != Item)
	{
		Item->OnItemDestroyed.AddUObject(this, &ThisClass::OnItemDestroyed);
		Items.Add(Item);
	}

	if (nullptr != ItemActor)
	{
		ItemActor->FinishSpawning(Transform);
	}
}

void AMCOMonsterSpawner::OnItemDestroyed()
{
	//SetState(EStageState::NEXT);
	
	
}
