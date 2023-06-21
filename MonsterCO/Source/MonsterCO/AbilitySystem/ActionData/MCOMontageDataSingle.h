#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "MCOActionData.h"
#include "MCOMontageDataSingle.generated.h"

class UMCOActionDefinition;

UCLASS()
class MONSTERCO_API UMCOMontageDataSingle : public UMCOActionData
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage;


};
