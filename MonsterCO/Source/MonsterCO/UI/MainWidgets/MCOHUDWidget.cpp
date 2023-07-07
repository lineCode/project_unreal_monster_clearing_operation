#include "MCOHUDWidget.h"
#include "Interface/MCOGameModeInterface.h"
#include "UI/SubWidgets/MCOHpWidget.h"
#include "UI/SubWidgets/MCOAttributeWidget.h"
#include "UI/SubWidgets/MCOSkillWidget.h"
#include "UI/SubWidgets/MCOSkillWidgetData.h"
#include "Interface/MCOHUDInterface.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "UI/SubWidgets/MCOStageWidget.h"


UMCOHUDWidget::UMCOHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// GETASSET(SkillWidgetData, UMCOSkillWidgetData, TEXT("/Game/Data/Player/DA_SkillWidgetData.DA_SkillWidgetData"));

	PlayerName = FName("Player");
	MonsterName = FName("Monster");
}

void UMCOHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetInGameWidget(PlayerName);
	SetInGameWidget(MonsterName);
	
	StageWidget = Cast<UMCOStageWidget>(GetWidgetFromName(TEXT("WBP_StageInfo")));
	ensure(nullptr != StageWidget);

	// for (int32 i = 0; i < SLOT_MAX; i++)
	// {
	// 	SkillWidgets.Emplace(Cast<UMCOSkillWidget>(GetWidgetFromName(
	// 		*FString::Printf(TEXT("WBP_PlayerSkill_%d"), i + 1)
	// 	)));
	// 	
	// 	ensure(SkillWidgets[i]);
	// }

	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
	GameModeInterface->GetOnStageChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnStageChanged);
	
	IMCOHUDInterface* HUDPawn = Cast<IMCOHUDInterface>(GetOwningPlayerPawn());
	ensure(nullptr != HUDPawn);
	HUDPawn->InitializeHUD(this);
}

void UMCOHUDWidget::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::RESTART_STAGE_AFTER_LOSE)
	{
		IMCOHUDInterface* HUDPawn = Cast<IMCOHUDInterface>(GetOwningPlayerPawn());
		ensure(nullptr != HUDPawn);
		HUDPawn->InitializeHUD(this);
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		ShowInGameWidget(PlayerName, true);
		ShowInGameWidget(MonsterName, false);
	}
}

void UMCOHUDWidget::OnMonsterFirstHit()
{
	ShowInGameWidget(MonsterName, true);
}

void UMCOHUDWidget::SetInGameWidget(const FName& InName)
{
	UWidget* HpWidget = GetWidgetFromName(*FString::Printf(TEXT("WBP_%sHpBar"), *InName.ToString()));
	ensure(nullptr != HpWidget);
	HpWidgets.Emplace(InName, Cast<UMCOHpWidget>(HpWidget));

#if WITH_EDITOR
	UWidget* AttributeWidget = GetWidgetFromName(*FString::Printf(TEXT("WBP_%sAttribute"), *InName.ToString()));
	ensure(nullptr != AttributeWidget);
	AttributeWidgets.Emplace(InName, Cast<UMCOAttributeWidget>(AttributeWidget));
#endif
}

void UMCOHUDWidget::OnStageChanged(const int32& InCurrent)
{
	StageWidget->OnStageChanged(InCurrent);
}

void UMCOHUDWidget::ShowInGameWidget(const FName& InName, bool bShow)
{
	ISTRUE(true == HpWidgets.Contains(InName));
	ISTRUE(nullptr != HpWidgets[InName]);
	HpWidgets[InName]->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

#if WITH_EDITOR
	ISTRUE(true == AttributeWidgets.Contains(InName));
	ISTRUE(nullptr != AttributeWidgets[InName]);
	
	AttributeWidgets[InName]->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
#endif
}

UMCOHpWidget* UMCOHUDWidget::GetHpWidget(const bool bIsPlayer)
{
	const FName& CurName = (true == bIsPlayer) ? PlayerName : MonsterName;
	ISTRUE_N(true == HpWidgets.Contains(CurName));
	return HpWidgets[CurName];
}

UMCOAttributeWidget* UMCOHUDWidget::GetAttributeWidget(const bool bIsPlayer)
{
	const FName& CurName = (true == bIsPlayer) ? PlayerName : MonsterName;
	ISTRUE_N(true == AttributeWidgets.Contains(CurName));
	return AttributeWidgets[CurName];
}


// void UMCOHUDWidget::InitializeSkillWidget(const FGameplayTag& InTag, const FText& InKeyText)
// {
// 	// get an empty slot then apply
// 	for (uint8 i = 0; i < SkillWidgets.Num(); i++)
// 	{
// 		if (false == SkillWidgets[i]->IsActive())
// 		{
// 			SkillWidgetData->SetData(InTag, i, InKeyText);
// 			SkillWidgets[i]->SetSkillWidget(SkillWidgetData->GetTexture(InTag), InKeyText);
// 			return;
// 		}
// 	}
//
// 	MCOLOG(TEXT("not enough slot"));
// }
//
// void UMCOHUDWidget::StartSkillWidget(const FGameplayTag& InTag, const float& InCooldownTime)
// {
// 	int8 idx = SkillWidgetData->GetIndex(InTag);
// 	ISTRUE(0 <= idx);
// 	ensure(idx < SkillWidgets.Num());
// 	SkillWidgets[idx]->StartSkillCooldown(InCooldownTime);
// }
