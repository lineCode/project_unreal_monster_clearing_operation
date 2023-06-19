#pragma once

#include "MonsterCO.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_AttackTiming.generated.h"


class UNiagaraSystem;


USTRUCT(BlueprintType)
struct FMCOAttackTimingData
{
	GENERATED_BODY()

public:
	FMCOAttackTimingData() : bIsMovableWhileGivingDamage(false) {};
	
	UPROPERTY(EditAnywhere)
	uint8 bIsMovableWhileGivingDamage : 1;
	
	UPROPERTY(EditAnywhere)
	float Begin = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float End = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float CheckRate = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DamagedNiagara = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Stiffness = 0.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 0.0f;
};


UCLASS()
class MONSTERCO_API UMCOActionFragment_AttackTiming : public UMCOActionFragment
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontageFrameRate = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMCOAttackTimingData> DamageTimings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NextComboFrameCount = 0.0f;
	
public:
	bool IsMovableWhileGivingDamage(const uint8& InDamageIdx) const;
	float GetDamageCheckRate(const uint8& InDamageIdx) const;
	float GetDamageBeginTimeAfterPrevEndTime(const uint8& InDamageIdx, const float& SpeedRate = 1.0f) const;
	float GetDamageExistTime(const uint8& InDamageIdx, const float& SpeedRate = 1.0f) const;
	float GetComboCheckTime(const float& SpeedRate = 1.0f) const;
	UNiagaraSystem* GetDamageNiagara(const uint8& InDamageIdx) const;
	float GetStiffness(const uint8& InDamageIdx) const;
	float GetDamage(const uint8& InDamageIdx) const;

protected:
	float CalculateTime(const float& FrameCount, const float& SpeedRate = 1.0f) const;
};
