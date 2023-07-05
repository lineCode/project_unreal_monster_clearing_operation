#include "Projectile/MCOProjectileSpawnComponent.h"
#include "MCOProjectile.h"
#include "GameFramework/Character.h"

UMCOProjectileSpawnComponent::UMCOProjectileSpawnComponent()
{
}


void UMCOProjectileSpawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMCOProjectileSpawnComponent::BeginDestroy()
{
	for (auto Tuple : Pool)
	{
		for (AMCOProjectile* Projectile : Tuple.Value.Projectiles)
		{
			Projectile->Destroy();
		}
	}
	
	Super::BeginDestroy();
}

AMCOProjectile* UMCOProjectileSpawnComponent::SpawnProjectile(const TSubclassOf<AMCOProjectile>& InClass, ACharacter* InOwner, const FTransform& InTransform, const float& InSpeed, const float& InLifeSpan)
{
	if (false == Pool.Contains(InClass))
	{
		Pool.Emplace(InClass, FMCOProjectileArray());
	}
	
	if (Pool[InClass].Projectiles.IsEmpty())
	{
		return SpawnNewProjectile(InClass, InOwner, InTransform, InSpeed, InLifeSpan);
	}
	else
	{
		return RespawnProjectile(InClass, InOwner, InTransform, InSpeed, InLifeSpan);
	}
}

AMCOProjectile* UMCOProjectileSpawnComponent::SpawnNewProjectile(const TSubclassOf<AMCOProjectile>& InClass, ACharacter* InOwner, const FTransform& InTransform, const float& InSpeed, const float& InLifeSpan)
{
	MCOLOG(TEXT("[%s] Spawn new"), *InClass->GetName());
	
	AMCOProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AMCOProjectile>(
		InClass,
		InTransform,
		InOwner,
		InOwner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	ISTRUE_N(nullptr != SpawnedProjectile);
	
	SpawnedProjectile->ProjectileBackToPoolDelegate.AddUniqueDynamic(this, &ThisClass::BackToPool);
	SpawnedProjectile->Initialize(InSpeed, InLifeSpan);
	
	SpawnedProjectile->FinishSpawning(InTransform);
	
	return SpawnedProjectile;
}

AMCOProjectile* UMCOProjectileSpawnComponent::RespawnProjectile(const TSubclassOf<AMCOProjectile>& InClass, ACharacter* InOwner, const FTransform& InTransform, const float& InSpeed, const float& InLifeSpan)
{
	MCOLOG(TEXT("[%s] Respawn"), *InClass->GetName());
	
	ensure(true == Pool.Contains(InClass));
	ensure(0 < Pool[InClass].Projectiles.Num());
	
	AMCOProjectile* SpawnedProjectile = Pool[InClass].Projectiles.Pop();
	ensure(nullptr != SpawnedProjectile);

	SpawnedProjectile->SetActorTransform(InTransform);
	SpawnedProjectile->Initialize(InSpeed, InLifeSpan);
	SpawnedProjectile->OnRespawned();

	return SpawnedProjectile;
}

void UMCOProjectileSpawnComponent::BackToPool(AMCOProjectile* InProjectile)
{
	MCOLOG(TEXT("[%s] Back to pool : %s"), *InProjectile->GetClass()->GetName(), *InProjectile->GetName());
	
	Pool[InProjectile->GetClass()].Projectiles.Emplace(InProjectile);
}
