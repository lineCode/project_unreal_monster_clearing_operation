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

	bIsEquipped = true;
}

ACharacter* AMCOMonsterAttachment::GetAttackedCharacter()
{
	return Cast<ACharacter>(OwnerCharacter);
}