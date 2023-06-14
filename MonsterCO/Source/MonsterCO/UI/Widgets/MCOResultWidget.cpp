#include "MCOResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


UMCOResultWidget::UMCOResultWidget(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
}

void UMCOResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtResult")));
	ResultButtonTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtResultButton")));
	ResultButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnResult")));

	ensure(nullptr != ResultTxt);
	ensure(nullptr != ResultButtonTxt);
	ensure(nullptr != ResultButton);

	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UMCOResultWidget::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::LOBBY)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InState == EMCOGameState::RESULT_WIN)
	{
		SetVisibility(ESlateVisibility::Hidden);
		SetSuccessUI();
		SetVisibility(ESlateVisibility::Visible);
	}
	else if (InState == EMCOGameState::RESULT_LOSE)
	{
		SetVisibility(ESlateVisibility::Hidden);
		SetFailUI();
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UMCOResultWidget::SetSuccessUI()
{
	ISTRUE(nullptr != ResultTxt);
	ISTRUE(nullptr != ResultButtonTxt);
	
	ResultTxt->SetText(FText::FromString(TEXT("몬스터 소탕 완료!")));
	ResultButtonTxt->SetText(FText::FromString(TEXT("클릭하면 타이틀로")));
	ResultButton->OnClicked.Clear();
	ResultButton->OnClicked.AddDynamic(this, &ThisClass::BackToTitle);
}

void UMCOResultWidget::SetFailUI()
{
	ISTRUE(nullptr != ResultTxt);
	ISTRUE(nullptr != ResultButtonTxt);
	
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
