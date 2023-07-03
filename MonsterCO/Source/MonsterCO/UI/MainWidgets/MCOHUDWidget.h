#pragma once

#include "MonsterCO.h"
#include "MCOMainWidget.h"
#include "AbilitySystem/CharacterAbility/MCOCommonMontageData.h"
#include "MCOHUDWidget.generated.h"


class UMCOHpWidget;
class UMCOAttributeWidget;
class UMCOOptionWidget;
class UMCOStageWidget;



UCLASS()
class MONSTERCO_API UMCOHUDWidget : public UMCOMainWidget
{
	GENERATED_BODY()

public:
	UMCOHUDWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);

public:
	void OnMonsterFirstHit();
	
protected:
	void ShowInGameWidget(const FName& InName, bool bShow);
	void SetInGameWidget(const FName& InName);

protected:
	UFUNCTION()
	void OnStageChanged(const int32& InCurrent);
	
protected:
	UPROPERTY()
	FName PlayerName;
	
	UPROPERTY()
	FName MonsterName;
	
// --- HP Widget
public:
	UMCOHpWidget* GetHpWidget(const bool bIsPlayer);
	
protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOHpWidget>> HpWidgets;
	

// --- Attribute Widget
public:
	UMCOAttributeWidget* GetAttributeWidget(const bool bIsPlayer);
	
protected:
	UPROPERTY()
	TMap<FName, TObjectPtr<UMCOAttributeWidget>> AttributeWidgets;

	
// --- Stage Widget
protected:
	UPROPERTY()
	TObjectPtr<UMCOStageWidget> StageWidget;

	
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
