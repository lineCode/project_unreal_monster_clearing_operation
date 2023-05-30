#include "MCOPlayerModeComponent.h"
#include "GameFramework/Character.h"
#include "MCOWeapon.h"
#include "MCOPlayerModeData.h"


UMCOPlayerModeComponent::UMCOPlayerModeComponent()
{
	GETASSET(ModeData, UMCOPlayerModeData, TEXT("/Game/Data/Player/DA_PlayerModeData.DA_PlayerModeData"));
}

void UMCOPlayerModeComponent::SpawnWeapon(ACharacter* InOwner)
{
	ISTRUE(true == ModeData->WeaponClasses.Contains(CurrentModeType));
	ISTRUE(nullptr != ModeData->WeaponClasses[CurrentModeType]);

	FTransform transform;
	CurrentWeaponActor = InOwner->GetWorld()->SpawnActorDeferred<AMCOWeapon>(ModeData->WeaponClasses[CurrentModeType], transform, InOwner);
	ISTRUE(CurrentWeaponActor);

	UGameplayStatics::FinishSpawningActor(CurrentWeaponActor, transform);
}

void UMCOPlayerModeComponent::SetMode(const EMCOModeType InModeType)
{
	CurrentModeType = InModeType;

	// Change weapon actor, spawn.

	// MCOLOG(TEXT("Mode is changed to %s"), *FHelper::GetEnumDisplayName(TEXT("EMCOModeType"), (int32)CurrentModeType));
}

void UMCOPlayerModeComponent::SwitchEquipUnequip()
{
	ISTRUE(CurrentWeaponActor);

	bIsEquipped = bIsEquipped == false;

	CurrentWeaponActor->SwitchEquipUnequip(bIsEquipped);
}
