#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/CharacterAbility/MCOCommonMontageData.h"
#include "MCOHUDWidget.generated.h"


class UMCOHpWidget;
class UMCOAttributeWidget;
class UMCOSkillWidget;
class UMCOSkillWidgetData;
class UMCOResultWidget;


UCLASS()
class MONSTERCO_API UMCOHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMCOHUDWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);

	void OnMonsterFirstHit();
	void ShowInGameWidget(const FName& InName, bool bShow);

protected:
	void SetInGameWidget(const FName& InName);
	void ShowTitleWidget(const bool bShow);
	void ShowResultWidget(const bool bShow);

protected:
	UPROPERTY()
	FName PlayerName;
	
	UPROPERTY()
	FName MonsterName;

	
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
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOAttributeWidget>> AttributeWidgets;

	
// --- Title
protected:
	UPROPERTY()
	TObjectPtr<UWidget> TitleWidget;

	
// --- Result Widget
protected:
	UPROPERTY()
	TObjectPtr<UMCOResultWidget> ResultWidget;

	
// --- Skill Widget
// public:
// 	void InitializeSkillWidget(const FGameplayTag& InTag, const FText& InKeyText);
// 	void StartSkillWidget(const FGameplayTag& InTag, const float& InCooldownTime);
//
// protected:
// 	UPROPERTY()
// 	TArray<TObjectPtr<UMCOSkillWidget>> SkillWidgets;
//
// 	UPROPERTY()
// 	TObjectPtr<UMCOSkillWidgetData> SkillWidgetData;

	
};
