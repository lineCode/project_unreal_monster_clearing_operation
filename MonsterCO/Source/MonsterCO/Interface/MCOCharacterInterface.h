#pragma once

#include "MonsterCO.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "MCOCharacterInterface.generated.h"


class UMCOHpWidget;
class UMCOAttributeWidget;
class UNiagaraSystem;
class ACharacter;
class AActor;
class AMCOProjectile;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCollisionBeginOverlapDelegate,
												ACharacter*, InAttacker,
												ACharacter*, InOtherCharacter,
												const FHitResult&, SweepResult);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCollisionEndOverlapDelegate,
												ACharacter*, InAttacker,
												AActor*, InAttackCauser,
												ACharacter*, InOtherCharacter);


UINTERFACE(MinimalAPI)
class UMCOCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOCharacterInterface
{
	GENERATED_BODY()

// --- Action
public:
	virtual bool CanActivateAbility(const FGameplayTag& InTag) = 0;

	virtual void StopCharacterFromMoving(bool bStopMoving) = 0;
	virtual void StopCharacterFromTurning(bool bStopTuring) = 0;
	virtual void ChangedMovementMode(const EMovementMode& InMode) = 0;

// --- Information
public:
	virtual FVector GetSocketLocation(const FName& InSocketName) = 0;
	virtual FTransform GetSocketTransform(const FName& InSocketName) = 0;
	virtual float GetCapsuleRadius() const = 0;
	virtual bool IsCharacterOnGround() const = 0;

// --- Collision
public:
	virtual void GetIgnoreActors(TArray<AActor*>& OutIgnoreActors) const = 0;
	virtual void OnBeginCollision(const FCollisionBeginOverlapDelegate& InBeginDelegate, const FCollisionEndOverlapDelegate& InEndDelegate, const FName& InSocketName) = 0;
	virtual void OnEndCollision(const FName& InSocketName) = 0;

// --- Projectile
public:
	virtual AMCOProjectile* SpawnProjectile(const TSubclassOf<AMCOProjectile>& InClass,const FName& InSocketName, const float& InSpeed, const float& InLifeSpan) = 0;
	
	
// --- Damaged
public:
	virtual bool CheckCanBeDamaged(FGameplayTag InTag) = 0;
	virtual float GetDamagedDegreeThenSetZero() = 0;
	virtual void SetDamagedDegree(const float& InDegree) = 0;

	
// --- Die
public:
	virtual void Die() = 0;
	virtual void FinishDying() = 0;

	
// --- Widget
public:
	virtual void InitializeWidget(UMCOHpWidget* InHpWidget, UMCOAttributeWidget* InAttributeWidget) = 0;
};
