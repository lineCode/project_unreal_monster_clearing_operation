#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MCOTitleWidget.generated.h"


class UButton;


UCLASS()
class MONSTERCO_API UMCOTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMCOTitleWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void StartGame();

protected:
	UPROPERTY()
	TObjectPtr<UButton> TitleButton;
};
