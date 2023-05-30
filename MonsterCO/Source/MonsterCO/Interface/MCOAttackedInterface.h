#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCOAttackedInterface.generated.h"

class AMCOCharacter;


UINTERFACE(MinimalAPI)
class UMCOAttackedInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOAttackedInterface
{
	GENERATED_BODY()

public:
	virtual ACharacter* GetAttackedCharacter() = 0;
	
};
