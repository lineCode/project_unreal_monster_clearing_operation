#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MCOWidgetComponent.generated.h"

UCLASS()
class MONSTERCO_API UMCOWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
