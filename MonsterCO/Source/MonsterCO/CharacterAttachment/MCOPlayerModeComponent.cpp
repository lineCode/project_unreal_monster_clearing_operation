#include "MCOPlayerModeComponent.h"
#include "GameFramework/Character.h"
#include "Attachment/MCOWeapon.h"
#include "MCOPlayerModeData.h"


UMCOPlayerModeComponent::UMCOPlayerModeComponent()
{
	GETASSET(ModeData, UMCOPlayerModeData, TEXT("/Game/Data/Player/DA_PlayerModeData.DA_PlayerModeData"));
}

void UMCOPlayerModeComponent::SpawnAttachment(ACharacter* InOwner)
{
	const EMCOPlayerMode& Mode = static_cast<EMCOPlayerMode>(GetCurrentMode());
	ISTRUE(true == ModeData->WeaponClasses.Contains(Mode));
	ISTRUE(nullptr != ModeData->WeaponClasses[Mode]);

	SpawnAttachmentByClass(InOwner, ModeData->WeaponClasses[Mode]);

	bIsToEquip = true;
}

void UMCOPlayerModeComponent::SetEquipUnequipInstantly(const bool bEquip)
{
	ISTRUE(bEquip == bIsToEquip);

	AMCOAttachment* Attachment = GetCurrentAttachment();
	ISTRUE(Attachment);
	AMCOWeapon* Weapon = Cast<AMCOWeapon>(Attachment);
	ISTRUE(Weapon);
	
	Weapon->SetEquipUnequipInstantly(bEquip);
	
	bIsToEquip = bEquip == false;
}

void UMCOPlayerModeComponent::SwitchEquipUnequip()
{
	AMCOAttachment* Attachment = GetCurrentAttachment();
	ISTRUE(Attachment);
	AMCOWeapon* Weapon = Cast<AMCOWeapon>(Attachment);
	ISTRUE(Weapon);
	
	Weapon->SwitchEquipUnequip(bIsToEquip);
	bIsToEquip = bIsToEquip == false;
}
