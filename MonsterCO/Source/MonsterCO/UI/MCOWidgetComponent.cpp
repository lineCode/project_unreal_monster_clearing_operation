#include "UI/MCOWidgetComponent.h"
#include "MCOUserWidget.h"

void UMCOWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMCOUserWidget* MCOUserWidget = Cast<UMCOUserWidget>(GetWidget());
	if (nullptr != MCOUserWidget)
	{
		MCOUserWidget->SetOwningActor(GetOwner());
	}
}
