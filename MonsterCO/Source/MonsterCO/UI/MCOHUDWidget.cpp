#include "MCOHUDWidget.h"
#include "Interface/MCOGameModeInterface.h"
#include "Widgets/MCOHpWidget.h"
#include "Widgets/MCOAttributeWidget.h"
#include "Widgets/MCOSkillWidget.h"
#include "Widgets/MCOSkillWidgetData.h"
#include "Interface/MCOHUDInterface.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


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
	}
	else if (InState == EMCOGameState::RESTART_STAGE_AFTER_LOSE)
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
	else if (InState == EMCOGameState::REWARD)
	{
	}
	else if (InState == EMCOGameState::NEXT)
	{
		//ShowInGameWidget(PlayerName, false);
		//ShowInGameWidget(MonsterName, false);
	}
	else if (InState == EMCOGameState::RESULT_WIN || InState == EMCOGameState::RESULT_LOSE)
	{
		ShowInGameWidget(PlayerName, false);
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
	
	UWidget* AttributeWidget = GetWidgetFromName(*FString::Printf(TEXT("WBP_%sAttribute"), *InName.ToString()));
	ensure(nullptr != AttributeWidget);
	AttributeWidgets.Emplace(InName, Cast<UMCOAttributeWidget>(AttributeWidget));
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

void UMCOHUDWidget::ShowTitleWidget(const bool bShow)
{
	ISTRUE(nullptr != TitleWidget);
	
	TitleWidget->SetVisibility((true == bShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
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
