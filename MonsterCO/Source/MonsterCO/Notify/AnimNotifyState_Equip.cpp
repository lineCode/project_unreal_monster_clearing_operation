#include "AnimNotifyState_Equip.h"
#include "Interface/MCOPlayerInterface.h"


FString UAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
    return TEXT("Equip");
}

void UAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ISTRUE(MeshComp);
	ISTRUE(MeshComp->GetOwner());
	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(MeshComp->GetOwner());
	ISTRUE(PlayerInterface);

	PlayerInterface->BeginAnimation_Equip();
}

void UAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ISTRUE(MeshComp);
	ISTRUE(MeshComp->GetOwner());
	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(MeshComp->GetOwner());
	ISTRUE(PlayerInterface);
	
	PlayerInterface->EndAnimation_Equip();
}
