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
	void SetWidgetValues(const float& InCurrentValue, const float& InMaxStamina);
	void StartWidget(const float& InAdditiveValue);

protected:
	void StartUpdatingStaminaWidget();
	void UpdateStaminaProgressBar();
	void ResetTimer();
	void OnStaminaEnd();
	void OnStaminaFull();

protected:
	float StartValue;
	float AdditiveValue;
	float CurrentValue;
	float MaxValue;
	FTimerHandle StaminaTimerHandle;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetImagePercent(float InPercent);

	UPROPERTY()
	TObjectPtr<UImage> StaminaRadialProgressBar;
};
