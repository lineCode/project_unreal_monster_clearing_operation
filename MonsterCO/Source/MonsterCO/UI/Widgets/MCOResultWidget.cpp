#include "MCOResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


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
}

void UMCOResultWidget::SetResultWidget(const bool bWin)
{
	if (true == bWin)
	{
		OnSuccess();
	}
	else
	{
		OnFail();
	}
}

void UMCOResultWidget::OnSuccess()
{
	ISTRUE(nullptr != ResultTxt);
	ISTRUE(nullptr != ResultButtonTxt);
	
	ResultTxt->SetText(FText::FromString(TEXT("몬스터 소탕 완료!")));
	ResultButtonTxt->SetText(FText::FromString(TEXT("클릭하면 타이틀로")));
	// ResultButton->OnClicked.AddDynamic(this, &ThisClass::SetResultWidget);
}

void UMCOResultWidget::OnFail()
{
	ISTRUE(nullptr != ResultTxt);
	ISTRUE(nullptr != ResultButtonTxt);
	
	ResultTxt->SetText(FText::FromString(TEXT("YOU DIED")));
	ResultButtonTxt->SetText(FText::FromString(TEXT("클릭하면 다시 싸우기")));
	// ResultButton->OnClicked.AddDynamic(this, &ThisClass::SetResultWidget);
}
