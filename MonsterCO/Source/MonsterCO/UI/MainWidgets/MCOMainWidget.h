#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MCOMainWidget.generated.h"


DECLARE_DELEGATE(FOnWidgetDelegate);
DECLARE_DELEGATE(FOnWidgetClosedByUserDelegate);


UCLASS()
class MONSTERCO_API UMCOMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	FOnWidgetDelegate OnExitGameDelegate;
	FOnWidgetClosedByUserDelegate OnWidgetClosedByUserDelegate;

	virtual void OnShow();
	void OnClosed();
	
protected:
	UFUNCTION()
	void ExitGame();
};
