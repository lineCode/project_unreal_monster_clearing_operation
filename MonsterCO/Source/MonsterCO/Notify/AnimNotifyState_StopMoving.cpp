#include "Notify/AnimNotifyState_StopMoving.h"

#include "Interface/MCOCharacterInterface.h"

FString UAnimNotifyState_StopMoving::GetNotifyName_Implementation() const
{
    return TEXT("StopMoving");
}

void UAnimNotifyState_StopMoving::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ISTRUE(MeshComp);
	ISTRUE(MeshComp->GetOwner());
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MeshComp->GetOwner());
	ISTRUE(CharacterInterface);

	CharacterInterface->StopCharacter(true);
}

void UAnimNotifyState_StopMoving::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ISTRUE(MeshComp);
	ISTRUE(MeshComp->GetOwner());
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(MeshComp->GetOwner());
	ISTRUE(CharacterInterface);

	CharacterInterface->StopCharacter(false);
}
