#include "MCOResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


void UMCOResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtResult")));
	ResultButtonTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtResultButton")));
	ResultButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnResult")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnExit")));

	ensure(nullptr != ResultTxt);
	ensure(nullptr != ResultButtonTxt);
	ensure(nullptr != ResultButton);
	ensure(nullptr != ExitButton);

	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitGame);
}

void UMCOResultWidget::OnShow()
{
	Super::OnShow();

	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	const EMCOGameState& GameState = GameModeInterface->GetGameState();

	if (GameState == EMCOGameState::RESULT_WIN)
	{
		OnSuccessUI();
	}
	else if (GameState == EMCOGameState::RESULT_LOSE)
	{
		OnFailUI();
	}
}

void UMCOResultWidget::OnSuccessUI()
{
	ISTRUE(nullptr != ResultTxt);
	ISTRUE(nullptr != ResultButtonTxt);
	ISTRUE(nullptr != ResultButton);
	
	ResultTxt->SetText(FText::FromString(TEXT("몬스터 소탕 완료!")));
	ResultButtonTxt->SetText(FText::FromString(TEXT("클릭하면 타이틀로")));
	ResultButton->OnClicked.Clear();
	ResultButton->OnClicked.AddDynamic(this, &ThisClass::BackToTitle);
}

void UMCOResultWidget::OnFailUI()
{
	ISTRUE(nullptr != ResultTxt);
	ISTRUE(nullptr != ResultButtonTxt);
	ISTRUE(nullptr != ResultButton);
	
	ResultTxt->SetText(FText::FromString(TEXT("YOU DIED")));
	ResultButtonTxt->SetText(FText::FromString(TEXT("클릭하면 다시 싸우기")));
	ResultButton->OnClicked.Clear();
	ResultButton->OnClicked.AddDynamic(this, &ThisClass::RestartStage);
}

void UMCOResultWidget::BackToTitle()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UMCOResultWidget::RestartStage()
{
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::RESTART_STAGE_AFTER_LOSE);
}
