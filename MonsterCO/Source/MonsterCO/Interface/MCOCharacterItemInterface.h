#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCOCharacterItemInterface.generated.h"

class UMCOActionFragment_AttributeEffect;
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
	virtual UMCOActionFragment_AttributeEffect* GetItemAttributeFragment() = 0;
	virtual void EndTakeItem() = 0;
};
