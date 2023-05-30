#include "MCOMonsterAttachment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"


AMCOMonsterAttachment::AMCOMonsterAttachment()
{
}

void AMCOMonsterAttachment::BeginPlay()
{
	Super::BeginPlay();

}

void AMCOMonsterAttachment::TurnOnCollision(const FName& InName)
{
	ISTRUE(true == ShapeComponentsMap.Contains(InName));
	
	ShapeComponentsMap[InName]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMCOMonsterAttachment::TurnOffCollision(const FName& InName)
{
	ISTRUE(true == ShapeComponentsMap.Contains(InName));
	
	ShapeComponentsMap[InName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

ACharacter* AMCOMonsterAttachment::GetAttackedCharacter()
{
	return Cast<ACharacter>(OwnerCharacter);
}
