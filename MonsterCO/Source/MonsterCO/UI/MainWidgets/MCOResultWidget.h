#pragma once

#include "MonsterCO.h"
#include "MCOMainWidget.h"
#include "MCOResultWidget.generated.h"


class UTextBlock;
class UButton;


UCLASS()
class MONSTERCO_API UMCOResultWidget : public UMCOMainWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	virtual void OnShow() override;
	
	void OnSuccessUI();
	void OnFailUI();
	
protected:
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
	TObjectPtr<UButton> ExitButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> ResultButtonTxt;
};
