#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "MCOResultWidget.generated.h"


class UTextBlock;
class UButton;


UCLASS()
class MONSTERCO_API UMCOResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMCOResultWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);

	void SetSuccessUI();
	void SetFailUI();

	void BackToTitle();
	void RestartStage();
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> ResultTxt;
	
	UPROPERTY()
	TObjectPtr<UButton> ResultButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> ResultButtonTxt;
};
