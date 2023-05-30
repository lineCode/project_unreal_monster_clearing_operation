#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCOHUDInterface.generated.h"

class UMCOHUDWidget;



UINTERFACE(MinimalAPI)
class UMCOHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetHUD(UMCOHUDWidget* InHUDWidget) = 0;
};
