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



DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCollisionBeginOverlapDelegate,
												ACharacter*, InAttacker,
												AActor*, InAttackCauser,
												ACharacter*, InOtherCharacter,
												const FHitResult&, SweepResult);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCollisionEndOverlapDelegate,
												ACharacter*, InAttacker,
												AActor*, InAttackCauser,
												ACharacter*, InOtherCharacter);



USTRUCT()
struct FMCODamagedData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float DamagedDegree = 0.0f;

	UPROPERTY()
	FVector DamagedLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> DamagedNiagara = nullptr;
	
	UPROPERTY()
	TObjectPtr<UNiagaraSystem> DurationNiagara = nullptr;
};


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

// --- Information
public:
	virtual FVector GetSocketLocation(const FName& InSocketName) = 0;
	virtual FTransform GetSocketTransform(const FName& InSocketName) = 0;

	virtual float GetCapsuleRadius() const = 0;

// --- Collision
public:
	virtual FCollisionBeginOverlapDelegate& GetCollisionBeginOverlapDelegate() = 0;
	virtual void TurnOnCollision(const FName& InName) = 0;
	virtual void TurnOffCollision(const FName& InName) = 0;

	
// --- Damaged
public:
	virtual const FMCODamagedData& GetDamagedData() = 0;
	virtual void SetDamagedData(const FMCODamagedData& InDegree) = 0;

	
// --- Die
public:
	virtual void Die() = 0;
	virtual void FinishDying() = 0;

	
// --- Widget
public:
	virtual void InitializeWidget(UMCOHpWidget* InHpWidget, UMCOAttributeWidget* InAttributeWidget) = 0;
};
