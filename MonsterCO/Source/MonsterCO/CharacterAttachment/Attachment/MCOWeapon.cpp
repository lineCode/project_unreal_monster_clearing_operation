#include "MCOWeapon.h"
#include "Components/SkeletalMeshComponent.h"


AMCOWeapon::AMCOWeapon()
{
	UnequipSocketName = FName(TEXT("Holster_GreatSword"));
	EquipSocketName = FName(TEXT("Hand_GreatSword"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Scene);
	
}

void AMCOWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttachActorToSocket(UnequipSocketName);

	bIsEquipped = false;
}

void AMCOWeapon::SetEquipUnequipInstantly(bool bEquip)
{
	if (bEquip == true)
	{
		AttachActorToSocket(EquipSocketName);
	}
	else
	{
		AttachActorToSocket(UnequipSocketName);
	}
	bIsEquipped = bEquip;
}

void AMCOWeapon::SwitchEquipUnequip(bool bEquip)
{
	bToEquip = bEquip;
}

void AMCOWeapon::BeginAnimation_Equip()
{
	if (bToEquip == true)
	{
		AttachActorToSocket(EquipSocketName);
	}
}

void AMCOWeapon::EndAnimation_Equip()
{
	if (bToEquip == false)
	{
		AttachActorToSocket(UnequipSocketName);
	}

	bIsEquipped = bToEquip;
}
