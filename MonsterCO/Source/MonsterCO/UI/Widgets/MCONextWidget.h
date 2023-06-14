#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "MCONextWidget.generated.h"


class UTextBlock;




UCLASS()
class MONSTERCO_API UMCONextWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	UMCONextWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);

	void StartTimer();
	void UpdateTime();
	void MoveToNextStage();
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> TimeTxt;

	int32 CurrentTime;
	FTimerHandle NextStageTimerHandle;
};
