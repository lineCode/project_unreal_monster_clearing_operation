#pragma once

#include "CoreMinimal.h"
#include "MCOMontageDataBase.h"
#include "MCOMontageDataCombo.generated.h"

class UMCOMontageDataUnit;

UCLASS()
class MONSTERCO_API UMCOMontageDataCombo : public UMCOMontageDataBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<TObjectPtr<UMCOMontageDataUnit>> ComboMontageData;

public:
	UMCOMontageDataUnit* GetMontageData(const uint8 InComboIdx);
	UAnimMontage* GetMontage(const uint8 InComboIdx);
	uint8 GetMaxCombo() const;
	
};
