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

}

void UMCOHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetInGameWidget(GetCharacterName(true));
	SetInGameWidget(GetCharacterName(false));

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
	GameModeInterface->GetOnGameResultDelegate().AddUniqueDynamic(ResultWidget, &UMCOResultWidget::SetResultWidget);
	GameModeInterface->GetOnRestartStageDelegate().AddUniqueDynamic(this, &ThisClass::OnRestartStage);
	
	IMCOHUDInterface* HUDPawn = Cast<IMCOHUDInterface>(GetOwningPlayerPawn());
	ensure(nullptr != HUDPawn);
	HUDPawn->InitializeHUD(this);
}

void UMCOHUDWidget::OnGameStateChanged(const EMCOGameState& InState)
{
	FString PlayerName = GetCharacterName(true);
	FString MonsterName = GetCharacterName(false);
	
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
		ShowResultWidget(false);
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
	}
}

void UMCOHUDWidget::OnRestartStage()
{
	IMCOHUDInterface* HUDPawn = Cast<IMCOHUDInterface>(GetOwningPlayerPawn());
	ensure(nullptr != HUDPawn);
	HUDPawn->InitializeHUD(this);
}

void UMCOHUDWidget::OnMonsterFirstHit()
{
	FString MonsterName = GetCharacterName(false);
	ShowInGameWidget(MonsterName, true);
}

void UMCOHUDWidget::SetInGameWidget(const FString& InName)
{
	UWidget* HpWidget = GetWidgetFromName(*FString::Printf(TEXT("WBP_%sHpBar"), *InName));
	ensure(nullptr != HpWidget);
	HpWidgets.Emplace(InName, Cast<UMCOHpWidget>(HpWidget));
	
	UWidget* AttributeWidget = GetWidgetFromName(*FString::Printf(TEXT("WBP_%sAttribute"), *InName));
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

FString UMCOHUDWidget::GetCharacterName(const bool bIsPlayer)
{
	return (true == bIsPlayer) ? TEXT("Player") : TEXT("Monster");
}

void UMCOHUDWidget::ShowInGameWidget(const FString& InName, bool bShow)
{
	ISTRUE(true == HpWidgets.Contains(InName));
	ISTRUE(nullptr != HpWidgets[InName]);
	ISTRUE(true == AttributeWidgets.Contains(InName));
	ISTRUE(nullptr != AttributeWidgets[InName]);
	
	HpWidgets[InName]->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	AttributeWidgets[InName]->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

UMCOHpWidget* UMCOHUDWidget::GetHpWidget(const bool bIsPlayer)
{
	const FString& CurName = GetCharacterName(bIsPlayer);
	ISTRUE_N(true == HpWidgets.Contains(CurName));
	return HpWidgets[CurName];
}

UMCOAttributeWidget* UMCOHUDWidget::GetAttributeWidget(const bool bIsPlayer)
{
	const FString& CurName = GetCharacterName(bIsPlayer);
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
