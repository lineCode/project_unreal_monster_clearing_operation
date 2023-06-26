#pragma once

#include "MonsterCO.h"
#include "MCOMainWidget.h"
#include "MCONextWidget.generated.h"


class UTextBlock;




UCLASS()
class MONSTERCO_API UMCONextWidget : public UMCOMainWidget
{
	GENERATED_BODY()
		
public:
	UMCONextWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	virtual void OnShow() override;

protected:
	void StartTimer();
	void UpdateTime();
	void MoveToNextStage();
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> TimeTxt;

	int32 CurrentTime;
	FTimerHandle NextStageTimerHandle;
};
