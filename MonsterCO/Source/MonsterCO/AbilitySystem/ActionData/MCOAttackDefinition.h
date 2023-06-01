#pragma once

#include "CoreMinimal.h"
#include "MCOAttackDefinition.generated.h"


UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class MONSTERCO_API UMCOAttackFragment : public UObject
{
	GENERATED_BODY()
};


// not yet used
UCLASS(Blueprintable, Const, Abstract)
class MONSTERCO_API UMCOAttackDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	
};
