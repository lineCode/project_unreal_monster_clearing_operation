#pragma once

#include "MonsterCO.h"
#include "MCOActionDefinition.h"
#include "Projectile/MCOProjectile.h"
#include "MCOActionFragment_AttackTiming.generated.h"


class UNiagaraSystem;


USTRUCT(BlueprintType)
struct FMCOAttackTimingData
{
	GENERATED_BODY()

public:
	FMCOAttackTimingData() : bIsMovable(false), bCanTurn(true), bUseProjectile(false) {};
	
	UPROPERTY(EditAnywhere, Category = Control)
	uint8 bIsMovable : 1;
	
	UPROPERTY(EditAnywhere, Category = Control)
	uint8 bCanTurn : 1;
	
	UPROPERTY(EditAnywhere, Category = Timing)
	float Begin = 0.0f;
		
	UPROPERTY(EditAnywhere, Category = Timing)
	float End = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = Timing)
	float CheckRate = 0.0f; // Check multiple times in every check rate (from Begin time to End time)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute)
	TObjectPtr<UMCOActionFragment_AttributeEffect> Attribute;
	
	UPROPERTY(EditAnywhere, Category = Projectile)
	uint8 bUseProjectile : 1;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile)
	TSubclassOf<AMCOProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float ProjectileLifeSpan = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float ProjectileSpeed = 600.0f;
	
};


UCLASS()
class MONSTERCO_API UMCOActionFragment_AttackTiming : public UMCOActionFragment
{
	GENERATED_BODY()
	
protected:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontageFrameRate = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMCOAttackTimingData> DamageTimings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NextComboFrameCount = 0.0f;

public:
	bool IsValidIdx(const uint8& InDamageIdx) const;
	
// --- Control
public:
	bool IsMovable(const uint8& InDamageIdx) const;
	bool CanTurn(const uint8& InDamageIdx) const;

// --- Timing
public:
	float GetDamageCheckRate(const uint8& InDamageIdx) const;
	float GetDamageBeginTimeAfterPrevEndTime(const uint8& InDamageIdx, const float& SpeedRate = 1.0f) const;
	float GetDamageExistTime(const uint8& InDamageIdx, const float& SpeedRate = 1.0f) const;
	float GetComboCheckTime(const float& SpeedRate = 1.0f) const;

protected:
	float CalculateTime(const float& FrameCount, const float& SpeedRate = 1.0f) const;

// --- Damage
public:
	UMCOActionFragment_AttributeEffect* GetAttributeFragment(const uint8& InDamageIdx) const;
	UNiagaraSystem* GetDamageNiagara(const uint8& InDamageIdx) const;
	UNiagaraSystem* GetDurationEffectNiagara(const uint8& InDamageIdx) const;

// --- Projectile
public:
	bool IsUsingProjectile(const uint8& InDamageIdx) const;
	const TSubclassOf<AMCOProjectile>* GetProjectileClass(const uint8& InDamageIdx) const;
	float GetProjectileSpeed(const uint8& InDamageIdx) const;
	float GetProjectileLifeSpan(const uint8& InDamageIdx) const;

};
