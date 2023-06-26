#include "MCOOptionWidget.h"
#include "Components/Button.h"



void UMCOOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ContinueButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnContinue")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnExit")));

	ensure(nullptr != ContinueButton);
	ensure(nullptr != ExitButton);

	ContinueButton->OnClicked.AddDynamic(this, &ThisClass::ContinueGame);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitGame);
}

void UMCOOptionWidget::ContinueGame()
{
	OnWidgetClosedByUserDelegate.ExecuteIfBound();
}
