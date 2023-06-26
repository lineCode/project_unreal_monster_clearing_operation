#pragma once

#include "MonsterCO.h"
#include "MCOMainWidget.h"
#include "MCOOptionWidget.generated.h"


class UButton;



UCLASS()
class MONSTERCO_API UMCOOptionWidget : public UMCOMainWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ContinueGame();

protected:
	UPROPERTY()
	TObjectPtr<UButton> ContinueButton;
	
	UPROPERTY()
	TObjectPtr<UButton> ExitButton;
};
