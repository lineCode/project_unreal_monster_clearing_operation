#include "MCOMonsterModeComponent.h"
#include "GameFramework/Character.h"
#include "MCOMonsterAttachment.h"


UMCOMonsterModeComponent::UMCOMonsterModeComponent()
{
	GETCLASS(AttachmentClass, AMCOMonsterAttachment, TEXT("/Game/Monsters/Dragon/BP_DragonAttachment.BP_DragonAttachment_C"));
}

void UMCOMonsterModeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMCOMonsterModeComponent::SpawnAttachment(ACharacter* InOwner)
{
	FTransform transform;

	Attachment = InOwner->GetWorld()->SpawnActorDeferred<AMCOMonsterAttachment>(AttachmentClass, transform, InOwner);
	ISTRUE(Attachment);

	UGameplayStatics::FinishSpawningActor(Attachment, transform);
}
