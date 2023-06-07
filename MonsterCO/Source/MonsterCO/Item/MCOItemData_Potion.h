#pragma once

#include "CoreMinimal.h"
#include "MCOItemData.h"
#include "MCOItemData_Potion.generated.h"

class UMCOActionFragment_Attribute;

UCLASS()
class MONSTERCO_API UMCOItemData_Potion : public UMCOItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_Attribute> AttributeFragment;
};
