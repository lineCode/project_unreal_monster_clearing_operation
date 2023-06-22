#pragma once

#include "MonsterCO.h"
#include "UI/MCOUserWidget.h"
#include "MCOStaminaWidget.generated.h"

class UImage;

UCLASS()
class MONSTERCO_API UMCOStaminaWidget : public UMCOUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);
	
public:
	void SetPercent(const float& InPercent);
	
protected:
	void OnStaminaFull();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetImagePercent(float InPercent);

	UPROPERTY()
	TObjectPtr<UImage> StaminaRadialProgressBar;
	
	bool bIsTimerTicking = false;
	FTimerHandle StaminaTimerHandle;
};
