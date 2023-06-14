#include "UI/Widgets/MCONextWidget.h"
#include "Components/TextBlock.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


UMCONextWidget::UMCONextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentTime = 0;
}

void UMCONextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TimeTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtTime")));
	ensure(nullptr != TimeTxt);
	
	SetVisibility(ESlateVisibility::Hidden);

	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
	
}

void UMCONextWidget::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::MOVE_TO_NEXT_STAGE)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InState == EMCOGameState::NEXT)
	{
		SetVisibility(ESlateVisibility::Visible);
		StartTimer();
	}
}

void UMCONextWidget::StartTimer()
{
	TimeTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), NEXT_STAGE_TIME)));
	
	CurrentTime = 0;
	
	NextStageTimerHandle.Invalidate();
	
	GetWorld()->GetTimerManager().SetTimer(
		NextStageTimerHandle,
		this,
		&ThisClass::UpdateTime,
		1.0f,
		true,
		0.0f
	);
}

void UMCONextWidget::UpdateTime()
{
	if (NEXT_STAGE_TIME <= CurrentTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(NextStageTimerHandle);
		MoveToNextStage();
		return;
	}
	
	CurrentTime++;
	
	TimeTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), NEXT_STAGE_TIME - CurrentTime)));
}

void UMCONextWidget::MoveToNextStage()
{
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::MOVE_TO_NEXT_STAGE);
}
