#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Timer.generated.h"


class UNiagaraSystem;


USTRUCT(BlueprintType)
struct FMCOFrameCount
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Begin = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float End = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DamagedNiagara = nullptr;
};


UCLASS()
class MONSTERCO_API UMCOActionFragment_Timer : public UMCOActionFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MontageFrameRate = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMCOFrameCount> DamageTimings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NextComboFrameCount = 0.0f;
	
public:
	float GetDamageBeginTimeAfterPrevEndTime(uint8 InDamageIdx, float SpeedRate = 1.0f) const;
	float GetDamageExistTime(uint8 InDamageIdx, float SpeedRate = 1.0f) const;
	float GetComboCheckTime(float SpeedRate = 1.0f) const;
	UNiagaraSystem* GetDamageNiagara(uint8 InDamageIdx) const;

protected:
	float CalculateTime(float FrameCount, float SpeedRate = 1.0f) const;
};
