#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "MCOStageWidget.generated.h"


class UTextBlock;



UCLASS()
class MONSTERCO_API UMCOStageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void OnStageChanged(const int32& InCurrent) const;

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> CurrentStageText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxStageText;
};
