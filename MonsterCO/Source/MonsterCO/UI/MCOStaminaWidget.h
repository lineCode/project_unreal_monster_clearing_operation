#pragma once

#include "MonsterCO.h"
#include "MCOUserWidget.h"
#include "MCOStaminaWidget.generated.h"

class UImage;

UCLASS()
class MONSTERCO_API UMCOStaminaWidget : public UMCOUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetPercent(const float& InPercent);
	
protected:
	void OnStaminaFull();

protected:
	FTimerHandle StaminaTimerHandle;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetImagePercent(float InPercent);

	UPROPERTY()
	TObjectPtr<UImage> StaminaRadialProgressBar;
};
