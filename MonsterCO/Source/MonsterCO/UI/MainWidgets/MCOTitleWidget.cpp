#include "MCOTitleWidget.h"
#include "Components/Button.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"



void UMCOTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnStart")));
	ensure(nullptr != StartButton);
	StartButton->OnClicked.AddDynamic(this, &ThisClass::StartGame);
	
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnExit")));
	ensure(nullptr != ExitButton);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitGame);
}

void UMCOTitleWidget::StartGame()
{
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::FIGHT);
}
