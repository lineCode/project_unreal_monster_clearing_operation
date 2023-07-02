#include "MCOWeapon.h"
#include "Components/SkeletalMeshComponent.h"


AMCOWeapon::AMCOWeapon()
{
	HolsterSocketName = FName(TEXT("Holster_GreatSword"));
	HandSocketName = FName(TEXT("Hand_GreatSword"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Scene);
	
}

void AMCOWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttachActorToSocket(HolsterSocketName);

	bIsEquipped = false;
}

void AMCOWeapon::SetEquipUnequipInstantly(bool bEquip)
{
	if (bEquip == true)
	{
		AttachActorToSocket(HandSocketName);
	}
	else
	{
		AttachActorToSocket(HolsterSocketName);
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
		AttachActorToSocket(HandSocketName);
	}
}

void AMCOWeapon::EndAnimation_Equip()
{
	if (bToEquip == false)
	{
		AttachActorToSocket(HolsterSocketName);
	}

	bIsEquipped = bToEquip;
}
