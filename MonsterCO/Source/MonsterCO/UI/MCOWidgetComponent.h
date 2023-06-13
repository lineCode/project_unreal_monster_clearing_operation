#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MCOWidgetComponent.generated.h"

UCLASS()
class MONSTERCO_API UMCOWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void InitWidget() override;
};
