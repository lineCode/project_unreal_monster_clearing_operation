#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOMontageDataCombo.generated.h"

class UMCOActionFragment_Montage;

UCLASS()
class MONSTERCO_API UMCOMontageDataCombo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FGameplayTag ActivationTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<TObjectPtr<UMCOActionFragment_Montage>> MontageFragments;
	
public:
	const UMCOActionFragment_Montage* GetMontageFragment(const uint8 InComboIdx);
	UAnimMontage* GetMontage(const uint8 InComboIdx);
	uint8 GetMaxCombo() const;
	
};
