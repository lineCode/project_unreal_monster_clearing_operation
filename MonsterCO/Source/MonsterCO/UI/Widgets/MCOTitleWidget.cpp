#include "UI/Widgets/MCOTitleWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "Interface/MCOGameModeInterface.h"

UMCOTitleWidget::UMCOTitleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMCOTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TitleButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnTitle")));
	ensure(nullptr != TitleButton);

	TitleButton->OnClicked.AddDynamic(this, &ThisClass::StartGame);
}

void UMCOTitleWidget::StartGame()
{
	SetVisibility(ESlateVisibility::Hidden);

	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::FIGHT);
}
