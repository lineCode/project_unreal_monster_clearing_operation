#pragma once

#include "MonsterCO.h"
#include "Components/ActorComponent.h"
#include "MCOProjectileSpawnComponent.generated.h"


class AMCOProjectile;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyProjectiles);



USTRUCT()
struct FMCOProjectileArray
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TObjectPtr<AMCOProjectile>> Projectiles = TArray<TObjectPtr<AMCOProjectile>>();
};


UCLASS()
class MONSTERCO_API UMCOProjectileSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMCOProjectileSpawnComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AMCOProjectile* SpawnProjectile(const TSubclassOf<AMCOProjectile>& InClass, ACharacter* InOwner, const FTransform& InTransform, const float& InSpeed, const float& InLifeSpan);
	
protected:
	AMCOProjectile* SpawnNewProjectile(const TSubclassOf<AMCOProjectile>& InClass, ACharacter* InOwner, const FTransform& InTransform, const float& InSpeed, const float& InLifeSpan);
	AMCOProjectile* RespawnProjectile(const TSubclassOf<AMCOProjectile>& InClass, ACharacter* InOwner, const FTransform& InTransform, const float& InSpeed, const float& InLifeSpan);

	UFUNCTION()
	void BackToPool(AMCOProjectile* InProjectile);
	
protected:
	FOnDestroyProjectiles OnDestroyProjectiles;
	
	UPROPERTY()
	TMap<TSubclassOf<AMCOProjectile>, FMCOProjectileArray> Pool;

};
