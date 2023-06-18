#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_AttackTiming.generated.h"


class UNiagaraSystem;


USTRUCT(BlueprintType)
struct FMCOAttackTimingData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Begin = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float End = 0.0f;
	
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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontageFrameRate = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMCOAttackTimingData> DamageTimings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NextComboFrameCount = 0.0f;
	
public:
	float GetDamageBeginTimeAfterPrevEndTime(uint8 InDamageIdx, float SpeedRate = 1.0f) const;
	float GetDamageExistTime(uint8 InDamageIdx, float SpeedRate = 1.0f) const;
	float GetComboCheckTime(float SpeedRate = 1.0f) const;
	UNiagaraSystem* GetDamageNiagara(uint8 InDamageIdx) const;
	float GetStiffness(uint8 InDamageIdx) const;
	float GetDamage(uint8 InDamageIdx) const;

protected:
	float CalculateTime(float FrameCount, float SpeedRate = 1.0f) const;
};
