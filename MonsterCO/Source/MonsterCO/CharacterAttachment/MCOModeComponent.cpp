#include "MCOModeComponent.h"
#include "Attachment/MCOAttachment.h"
#include "GameFramework/Character.h"

UMCOModeComponent::UMCOModeComponent()
{
}

void UMCOModeComponent::BeginPlay()
{
	Super::BeginPlay();
}

AMCOAttachment* UMCOModeComponent::GetCurrentAttachment() const
{
	ISTRUE_N(true == Attachments.IsValidIndex(CurrentAttachmentIdx))
	return Attachments[CurrentAttachmentIdx];
}

FCollisionBeginOverlapDelegate& UMCOModeComponent::GetAttachmentBeginOverlapDelegate() const
{
	AMCOAttachment* Attachment = GetCurrentAttachment();
	ensure(nullptr != Attachment);
	return Attachment->OnCollisionBeginOverlapDelegate;
}

int32 UMCOModeComponent::GetCurrentMode() const
{
	return CurrentAttachmentIdx;
}

void UMCOModeComponent::SetCurrentMode(const int32 InIdx)
{
	CurrentAttachmentIdx = InIdx;
	
	// Change weapon actor, spawn.
}

void UMCOModeComponent::SpawnAttachmentByClass(ACharacter* InOwner, const TSubclassOf<AMCOAttachment>& InAttachmentClass)
{
	const FTransform transform;

	AMCOAttachment* Attachment = InOwner->GetWorld()->SpawnActorDeferred<AMCOAttachment>(InAttachmentClass, transform, InOwner);
	ensure(nullptr != Attachment);

	UGameplayStatics::FinishSpawningActor(Attachment, transform);

	Attachments.Emplace(Attachment);
}

bool UMCOModeComponent::IsEquipped() const
{
	AMCOAttachment* Attachment = GetCurrentAttachment();
	ISTRUE_F(nullptr != Attachment);
	return Attachment->GetIsEquipped();
}
