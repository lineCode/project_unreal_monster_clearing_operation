#include "MCOMonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "MCOMonstersData.h"
#include "Character/Monster/MCOMonsterCharacter.h"
#include "Game/MCOGameModeBase.h"
#include "Item/MCOItem.h"


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

	const AMCOGameModeBase* MCOGameMode = Cast<AMCOGameModeBase>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != MCOGameMode);
	SpawnMonster(MCOGameMode->GetPhase());
}

void AMCOMonsterSpawner::SpawnMonster(const int32& InPhase)
{
	ISTRUE(InPhase < MonstersData->Monsters.Num());
	
	SpawnedMonster = GetWorld()->SpawnActorDeferred<AMCOMonsterCharacter>(MonstersData->Monsters[InPhase], GetTransform());
	ISTRUE(nullptr != SpawnedMonster);

	SpawnedMonster->OnCharacterDeathFinished.AddUniqueDynamic(this, &ThisClass::OnMonsterDied);
	SpawnedMonster->FinishSpawning(GetTransform());
}

void AMCOMonsterSpawner::OnMonsterDied(const AMCOCharacter* InCharacter)
{
	ensure(nullptr != InCharacter);

	FVector SpawnLocation = InCharacter->GetActorLocation();
	SpawnLocation.Z -= InCharacter->GetDefaultHalfHeight();
		
	SpawnItem(SpawnLocation);
}

void AMCOMonsterSpawner::SpawnItem(const FVector& InSpawnLocation)
{
	AActor* ItemActor = GetWorld()->SpawnActorDeferred<AMCOItem>(ItemClass, FTransform(InSpawnLocation));
		
	AMCOItem* Item = Cast<AMCOItem>(ItemActor);
	ensure(nullptr != Item);
	
	FVector RelocationByHeight = Item->GetActorLocation();
	RelocationByHeight.Z += Item->GetItemHalfHeight();
	Item->SetActorLocation(RelocationByHeight);
	
	Item->OnItemDestroyed.AddUObject(this, &ThisClass::OnItemDestroyed);
	Items.Add(Item);
	Item->FinishSpawning(Item->GetTransform());
}

void AMCOMonsterSpawner::OnItemDestroyed()
{
	//SetState(EStageState::NEXT);
	
	
}
