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

public:
	UFUNCTION()
	void SetResultWidget(const bool bWin);
	
protected:
	void SetSuccessUI();
	void SetFailUI();

	UFUNCTION()
	void BackToTitle();
	
	UFUNCTION()
	void RestartStage();
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> ResultTxt;
	
	UPROPERTY()
	TObjectPtr<UButton> ResultButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> ResultButtonTxt;
};
