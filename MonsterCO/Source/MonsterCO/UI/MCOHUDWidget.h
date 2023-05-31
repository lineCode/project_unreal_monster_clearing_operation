#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/CharacterAbility/MCOCommonMontageData.h"
#include "MCOHUDWidget.generated.h"

class UMCOHpWidget;
class UMCOAttributeWidget;
class UMCOSkillWidget;
class UMCOSkillWidgetData;


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

// --- HP Widget
public:
	UMCOHpWidget* GetHpWidget(bool bIsPlayer);
	
protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOHpWidget>> HpWidgets;
	

// --- Attribute Widget
public:
	UMCOAttributeWidget* GetAttributeWidget(bool bIsPlayer);
	
protected:
	const FName GetName(bool bIsPlayer) const;
	
protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOAttributeWidget>> AttributeWidgets;


// --- Skill Widget
public:
	void InitializeSkillWidget(const FGameplayTag& InTag, const FText& InKeyText);
	void StartSkillWidget(const FGameplayTag& InTag, const float& InCooldownTime);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UMCOSkillWidget>> SkillWidgets;

	UPROPERTY()
	TObjectPtr<UMCOSkillWidgetData> SkillWidgetData;
};
