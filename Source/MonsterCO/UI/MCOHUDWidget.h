#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MCOHUDWidget.generated.h"

class UMCOHpWidget;
class UMCOAttributeWidget;


UCLASS()
class MONSTERCO_API UMCOHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMCOHUDWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

public:
	void InitializeHUDWidget(bool bIsPlayer);
	void ShowWidget(bool bIsPlayer, bool bIsToShow);
	
	UMCOHpWidget* GetHpWidget(bool bIsPlayer);
	UMCOAttributeWidget* GetAttributeWidget(bool bIsPlayer);

protected:
	const FName GetName(bool bIsPlayer) const;
	
protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOHpWidget>> HpWidgets;
	
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOAttributeWidget>> AttributeWidgets;
};
