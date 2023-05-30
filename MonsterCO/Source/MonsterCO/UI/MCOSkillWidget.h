#pragma once

#include "CoreMinimal.h"
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
	void StartSkillCooldown(UTexture2D* InImage, const float& InMax);
	void UpdateCooldownProgressBar();
	void ResetCooldownTimer();

protected:
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
