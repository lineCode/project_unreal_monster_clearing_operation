#pragma once

#include "CoreMinimal.h"
#include "MCOMainWidget.h"
#include "MCOTitleWidget.generated.h"


class UButton;


UCLASS()
class MONSTERCO_API UMCOTitleWidget : public UMCOMainWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void StartGame();
	
protected:
	UPROPERTY()
	TObjectPtr<UButton> StartButton;
	
	UPROPERTY()
	TObjectPtr<UButton> ExitButton;
};
