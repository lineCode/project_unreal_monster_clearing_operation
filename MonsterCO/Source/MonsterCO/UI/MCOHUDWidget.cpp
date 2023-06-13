#include "MCOHUDWidget.h"
#include "Interface/MCOGameModeInterface.h"
#include "Widgets/MCOHpWidget.h"
#include "Widgets/MCOAttributeWidget.h"
#include "Widgets/MCOResultWidget.h"
#include "Widgets/MCOSkillWidget.h"
#include "Widgets/MCOSkillWidgetData.h"
#include "Interface/MCOHUDInterface.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/MCOTitleWidget.h"


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

	TitleWidget = GetWidgetFromName(TEXT("WBP_Title")); 
	ensure(nullptr != TitleWidget);
	
	ResultWidget = Cast<UMCOResultWidget>(GetWidgetFromName(TEXT("WBP_Result")));
	ensure(nullptr != ResultWidget);

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
	GameModeInterface->GetOnGameResultDelegate().AddDynamic(ResultWidget, &UMCOResultWidget::SetResultWidget);
	
	
	IMCOHUDInterface* HUDPawn = Cast<IMCOHUDInterface>(GetOwningPlayerPawn());
	ensure(nullptr != HUDPawn);
	HUDPawn->InitializeHUD(this);
}

void UMCOHUDWidget::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::LOBBY)
	{
		ShowTitleWidget(true);
		ShowInGameWidget(PlayerName, false);
		ShowInGameWidget(MonsterName, false);
		ShowResultWidget(false);
	}
	else if (InState == EMCOGameState::FIGHT)
	{
		ShowInGameWidget(PlayerName, true);
	}
	else if (InState == EMCOGameState::REWARD)
	{
		ShowInGameWidget(PlayerName, false);
		ShowInGameWidget(MonsterName, false);
	}
	else if (InState == EMCOGameState::RESULT)
	{
		ShowInGameWidget(PlayerName, false);
		ShowInGameWidget(MonsterName, false);
		ShowResultWidget(true);
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
	
	UWidget* AttributeWidget = GetWidgetFromName(*FString::Printf(TEXT("WBP_%sAttribute"), *InName.ToString()));
	ensure(nullptr != AttributeWidget);
	AttributeWidgets.Emplace(InName, Cast<UMCOAttributeWidget>(AttributeWidget));
}

void UMCOHUDWidget::ShowTitleWidget(const bool bShow)
{
	ISTRUE(nullptr != TitleWidget);
	
	TitleWidget->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMCOHUDWidget::ShowResultWidget(const bool bShow)
{
	ISTRUE(nullptr != ResultWidget);
	
	ResultWidget->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMCOHUDWidget::ShowInGameWidget(const FName& InName, bool bShow)
{
	ISTRUE(true == HpWidgets.Contains(InName));
	ISTRUE(nullptr != HpWidgets[InName]);
	ISTRUE(true == AttributeWidgets.Contains(InName));
	ISTRUE(nullptr != AttributeWidgets[InName]);
	
	HpWidgets[InName]->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	AttributeWidgets[InName]->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

UMCOHpWidget* UMCOHUDWidget::GetHpWidget(bool bIsPlayer)
{
	const FName& CurName = (true == bIsPlayer) ? PlayerName : MonsterName;
	ISTRUE_N(true == HpWidgets.Contains(CurName));
	return HpWidgets[CurName];
}

UMCOAttributeWidget* UMCOHUDWidget::GetAttributeWidget(bool bIsPlayer)
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
