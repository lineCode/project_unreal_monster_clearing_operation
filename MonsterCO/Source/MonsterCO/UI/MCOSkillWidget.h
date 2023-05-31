#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "MCOSkillWidget.generated.h"

class UImage;

UCLASS()
class MONSTERCO_API UMCOSkillWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	bool IsActive() { return bIsActive; } 
	void SetSkillWidget(UTexture2D* InTexture, const FText& InKeyText);
	void UnSetSkillWidget();
	void StartSkillCooldown(const float& InCooldownTime);
	void UpdateCooldownProgressBar();
	void ResetCooldownTimer();

protected:
	bool bIsActive = false;
	float MaxCooldownTime;
	FTimerHandle CooldownTimerHandle;
	FTimerHandle CooldownResetTimerHandle;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetImagePercent(float InPercent);

	UPROPERTY()
	TObjectPtr<UImage> Image;

	UPROPERTY()
	TObjectPtr<UImage> SkillRadialProgressBar;
	
};
