#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCOComboAttackData.generated.h"



class UMCOCommonMontageData;



USTRUCT(BlueprintType)
struct FMCOMontageComboData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMCOCommonMontageData> Data = nullptr;
	
	UPROPERTY(EditAnywhere)
	float NextComboFrameCount = 0.0f;
};

UCLASS(BlueprintType)
class MONSTERCO_API UMCOComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMCOComboAttackData();
	uint8 GetMaxCombo() const { return MontageData.Num(); }
	float GetComboCheckTime(uint8 CurrentCombo, float AttackSpeedRate = 1.0f);
	
	UMCOCommonMontageData* GetMontageData(uint8 CurrentCombo);
	UAnimMontage* GetMontage(uint8 CurrentCombo);

protected:
	UPROPERTY(EditAnywhere, Category = MontageData)	
	TArray<FMCOMontageComboData> MontageData;

};