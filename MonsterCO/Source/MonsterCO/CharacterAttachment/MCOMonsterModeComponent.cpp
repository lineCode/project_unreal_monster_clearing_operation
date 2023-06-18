#include "MCOMonsterModeComponent.h"
#include "GameFramework/Character.h"
#include "Attachment/MCOMonsterAttachment.h"


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
	SpawnAttachmentByClass(InOwner, AttachmentClass);
}
