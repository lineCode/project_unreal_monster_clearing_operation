#pragma once

#include "CoreMinimal.h"
#include "MCOAttackDefinition.h"
#include "MCOAttackFragment_Timer.generated.h"


USTRUCT(BlueprintType)
struct FMCOFrameCount
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Begin = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float End = 0.0f;
};




UCLASS()
class MONSTERCO_API UMCOAttackFragment_Timer : public UMCOAttackFragment
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
	float GetDamageBeginTimeAfterPrevEndTime(uint8 InDamageIdx, float SpeedRate = 1.0f);
	float GetDamageExistTime(uint8 InDamageIdx, float SpeedRate = 1.0f);
	float GetComboCheckTime(float SpeedRate = 1.0f);

protected:
	float CalculateTime(float FrameCount, float SpeedRate = 1.0f);
};
