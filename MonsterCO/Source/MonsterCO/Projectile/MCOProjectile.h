#pragma once

#include "MonsterCO.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOProjectile.generated.h"



class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileBeginOverlapDelegate,
												AActor*, OtherActor, 
												const FHitResult&, SweepResult);



UCLASS()
class MONSTERCO_API AMCOProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:
	void Initialize(const float& InSpeed, const float& InLifeSpan);
	
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	FCollisionBeginOverlapDelegate CollisionBeginOverlapDelegate;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	// UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	// FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	bool bIsHit;
};
