#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "MCOHpWidget.generated.h"

class UTextBlock;
class UProgressBar;


UCLASS()
class MONSTERCO_API UMCOHpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMCOHpWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void SetName(const FString& InName);

	UFUNCTION()
	void SetMaxHp(float NewMaxHp);
	
	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> NameText;
	
	UPROPERTY()
	TObjectPtr<UProgressBar> HpProgressBar;
	
	UPROPERTY()
	float MaxHp;
	
	UPROPERTY()
	float CurrentHp;
};
