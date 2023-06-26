#include "MCOAttributeWidget.h"
#include "Components/TextBlock.h"


void UMCOAttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHealth")));
	MaxHealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxHealth")));
	StaminaText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStamina")));
	MaxStaminaText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxStamina")));
	StiffnessText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStiffness")));
	MaxStiffnessText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxStiffness")));

	ensure(HealthText);
	ensure(MaxHealthText);
	ensure(StaminaText);
	ensure(MaxStaminaText);
	ensure(StiffnessText);
	ensure(MaxStiffnessText);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UMCOAttributeWidget::UpdateHealth(float InValue)
{
	ISTRUE(nullptr != HealthText);

	HealthText->SetText(
		FText::FromString(FString::SanitizeFloat(InValue))
	);
}

void UMCOAttributeWidget::UpdateMaxHealth(float InValue)
{
	ISTRUE(nullptr != MaxHealthText);

	MaxHealthText->SetText(
		FText::FromString(FString::SanitizeFloat(InValue))
	);
}

void UMCOAttributeWidget::UpdateStamina(float InValue)
{
	ISTRUE(nullptr != StaminaText);

	StaminaText->SetText(
		FText::FromString(FString::SanitizeFloat(InValue))
	);
}

void UMCOAttributeWidget::UpdateMaxStamina(float InValue)
{
	ISTRUE(nullptr != MaxStaminaText);

	MaxStaminaText->SetText(
		FText::FromString(FString::SanitizeFloat(InValue))
	);
}

void UMCOAttributeWidget::UpdateStiffness(float InValue)
{
	ISTRUE(nullptr != StiffnessText);

	StiffnessText->SetText(
		FText::FromString(FString::SanitizeFloat(InValue))
	);
}

void UMCOAttributeWidget::UpdateMaxStiffness(float InValue)
{
	ISTRUE(nullptr != MaxStiffnessText);

	MaxStiffnessText->SetText(
		FText::FromString(FString::SanitizeFloat(InValue))
	);
}

