#include "MCOAttributeWidget.h"
#include "Components/TextBlock.h"


void UMCOAttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHealth")));
	MaxHealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxHealth")));
	StiffnessText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStiffness")));
	MaxStiffnessText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxStiffness")));

	ensure(HealthText);
	ensure(MaxHealthText);
	ensure(StiffnessText);
	ensure(MaxStiffnessText);
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

