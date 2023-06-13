#include "UI/MCOWidgetComponent.h"
#include "MCOUserWidget.h"

void UMCOWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMCOWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMCOUserWidget* MCOUserWidget = Cast<UMCOUserWidget>(GetWidget());
	if (nullptr != MCOUserWidget)
	{
		MCOUserWidget->SetOwningActor(GetOwner());
	}
}
