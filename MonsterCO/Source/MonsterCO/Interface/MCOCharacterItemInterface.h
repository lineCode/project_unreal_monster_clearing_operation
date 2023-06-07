#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCOCharacterItemInterface.generated.h"

class UMCOActionFragment_Attribute;
class UMCOItemData;



UINTERFACE(MinimalAPI)
class UMCOCharacterItemInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOCharacterItemInterface
{
	GENERATED_BODY()

public:
	virtual void TakeItem(const UMCOItemData* InItemData) = 0;
	virtual UMCOActionFragment_Attribute* GetItemAttributeFragment() = 0;
	virtual void EndTakeItem() = 0;
};
