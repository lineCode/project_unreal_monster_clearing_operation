#include "MCOMonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "MCOMonstersData.h"
#include "Character/Monster/MCOMonsterCharacter.h"
#include "Game/MCOGameModeBase.h"


AMCOMonsterSpawner::AMCOMonsterSpawner()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	
	GETASSET(MonstersData, UMCOMonstersData, TEXT("/Game/Data/Monster/DA_MonstersData.DA_MonstersData"))
}

void AMCOMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	AMCOGameModeBase* MCOGameMode = Cast<AMCOGameModeBase>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != MCOGameMode);
	SpawnMonster(MCOGameMode->GetPhase());
}

void AMCOMonsterSpawner::SpawnMonster(int32 InPhase)
{
	ISTRUE(InPhase < MonstersData->Monsters.Num());
	
	SpawnedMonster = GetWorld()->SpawnActorDeferred<AMCOMonsterCharacter>(MonstersData->Monsters[InPhase], GetTransform());
	ISTRUE(nullptr != SpawnedMonster);
	
	SpawnedMonster->FinishSpawning(GetTransform());
}
