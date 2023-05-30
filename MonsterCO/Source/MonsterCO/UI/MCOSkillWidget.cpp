#include "UI/MCOSkillWidget.h"
#include "Components/Image.h"

void UMCOSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Image = Cast<UImage>(GetWidgetFromName(TEXT("SkillImage")));
	SkillRadialProgressBar = Cast<UImage>(GetWidgetFromName(TEXT("RadialProgressBar")));

	ensure(Image);
	ensure(SkillRadialProgressBar);
}

void UMCOSkillWidget::StartSkillCooldown(UTexture2D* InImage, const float& InMax)
{
	MaxCooldownTime = InMax;

	SkillRadialProgressBar->SetVisibility(ESlateVisibility::Visible);
	Image->SetBrushFromTexture(InImage);

	CooldownTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&ThisClass::UpdateCooldownProgressBar,
		0.1f,
		true,
		0.0f
	);
	
	CooldownResetTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(
		CooldownResetTimerHandle,
		this,
		&ThisClass::ResetCooldownTimer,
		MaxCooldownTime,
		false
	);
}

void UMCOSkillWidget::UpdateCooldownProgressBar()
{
	ensure(MaxCooldownTime != 0.0f);

	const float CurrentCooldownTime = GetWorld()->GetTimerManager().GetTimerRemaining(CooldownResetTimerHandle);
	SetImagePercent((MaxCooldownTime - CurrentCooldownTime) / MaxCooldownTime);
}

void UMCOSkillWidget::ResetCooldownTimer()
{
	SkillRadialProgressBar->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}
