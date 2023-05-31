#include "UI/MCOSkillWidget.h"
#include "Components/Image.h"

void UMCOSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Image = Cast<UImage>(GetWidgetFromName(TEXT("SkillImage")));
	SkillRadialProgressBar = Cast<UImage>(GetWidgetFromName(TEXT("RadialProgressBar")));

	ensure(Image);
	ensure(SkillRadialProgressBar);
	
	UnSetSkillWidget();
}

void UMCOSkillWidget::SetSkillWidget(UTexture2D* InTexture, const FText& InKeyText)
{
	Image->SetBrushFromTexture(InTexture);
	Image->SetVisibility(ESlateVisibility::Visible);
	SkillRadialProgressBar->SetVisibility(ESlateVisibility::Hidden);
	bIsActive = true;
}

void UMCOSkillWidget::UnSetSkillWidget()
{
	Image->SetVisibility(ESlateVisibility::Hidden);
	SkillRadialProgressBar->SetVisibility(ESlateVisibility::Hidden);
	bIsActive = false;
}

void UMCOSkillWidget::StartSkillCooldown(const float& InCooldownTime)
{
	MaxCooldownTime = InCooldownTime;

	SkillRadialProgressBar->SetVisibility(ESlateVisibility::Visible);

	CooldownTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&ThisClass::UpdateCooldownProgressBar,
		0.05f,
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
	SetImagePercent(0.0f);
	SkillRadialProgressBar->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}
