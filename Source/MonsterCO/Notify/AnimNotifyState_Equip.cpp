#include "AnimNotifyState_Equip.h"
#include "Player/MCOPlayerCharacter.h"
#include "PlayerWeapon/MCOWeapon.h"


FString UAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
    return TEXT("Equip");
}

void UAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ISTRUE(MeshComp);
	ISTRUE(MeshComp->GetOwner());
	AMCOPlayerCharacter* Player = Cast<AMCOPlayerCharacter>(MeshComp->GetOwner());
	ISTRUE(Player);
	AMCOWeapon* Weapon = Player->GetWeapon();
	ISTRUE(Weapon);

	Weapon->BeginAnimation_Equip();
}

void UAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ISTRUE(MeshComp);
	ISTRUE(MeshComp->GetOwner());
	AMCOPlayerCharacter* Player = Cast<AMCOPlayerCharacter>(MeshComp->GetOwner());
	ISTRUE(Player);
	AMCOWeapon* Weapon = Player->GetWeapon();
	ISTRUE(Weapon);

	Weapon->EndAnimation_Equip();
}
