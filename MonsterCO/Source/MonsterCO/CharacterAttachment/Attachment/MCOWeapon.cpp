#include "MCOWeapon.h"
#include "Components/SkeletalMeshComponent.h"


AMCOWeapon::AMCOWeapon()
{
	HolsterSocketName = FName(TEXT("Holster_GreatSword"));
	HandSocketName = FName(TEXT("Hand_GreatSword"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Scene);
	
	bControlCollision = true;
}

void AMCOWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttachActorToSocket(HolsterSocketName);

	bIsEquipped = false;
}

void AMCOWeapon::SwitchEquipUnequip(bool bInToEquip)
{
	bIsToEquip = bInToEquip;
}

void AMCOWeapon::BeginAnimation_Equip()
{
	if (bIsToEquip == true)
	{
		AttachActorToSocket(HandSocketName);
	}
}

void AMCOWeapon::EndAnimation_Equip()
{
	if (bIsToEquip == false)
	{
		AttachActorToSocket(HolsterSocketName);
	}

	bIsEquipped = bIsToEquip;
}
