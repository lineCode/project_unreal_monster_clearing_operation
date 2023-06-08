#include "MCOPlayerModeComponent.h"
#include "GameFramework/Character.h"
#include "MCOWeapon.h"
#include "MCOPlayerModeData.h"


UMCOPlayerModeComponent::UMCOPlayerModeComponent()
{
	GETASSET(ModeData, UMCOPlayerModeData, TEXT("/Game/Data/Player/DA_PlayerModeData.DA_PlayerModeData"));
}

bool UMCOPlayerModeComponent::IsEquipped() const
{
	ISTRUE_F(nullptr != CurrentWeaponActor);
	return CurrentWeaponActor->GetIsEquipped();
}

void UMCOPlayerModeComponent::SpawnWeapon(ACharacter* InOwner)
{
	ISTRUE(true == ModeData->WeaponClasses.Contains(CurrentModeType));
	ISTRUE(nullptr != ModeData->WeaponClasses[CurrentModeType]);

	FTransform transform;
	CurrentWeaponActor = InOwner->GetWorld()->SpawnActorDeferred<AMCOWeapon>(ModeData->WeaponClasses[CurrentModeType], transform, InOwner);
	ISTRUE(CurrentWeaponActor);

	bIsToEquip = true;

	UGameplayStatics::FinishSpawningActor(CurrentWeaponActor, transform);
}

void UMCOPlayerModeComponent::SetMode(const EMCOModeType InModeType)
{
	CurrentModeType = InModeType;

	// Change weapon actor, spawn.

	// MCOLOG(TEXT("Mode is changed to %s"), *FHelper::GetEnumDisplayName(TEXT("EMCOModeType"), (int32)CurrentModeType));
}

void UMCOPlayerModeComponent::SetEquip()
{
	ISTRUE(true == bIsToEquip);

	CurrentWeaponActor->SwitchEquipUnequip(true);
	CurrentWeaponActor->BeginAnimation_Equip();
	bIsToEquip = false;
}

void UMCOPlayerModeComponent::SwitchEquipUnequip()
{
	ISTRUE(CurrentWeaponActor);
	
	CurrentWeaponActor->SwitchEquipUnequip(bIsToEquip);
	bIsToEquip = bIsToEquip == false;
}
