#include "MCOHUDWidget.h"
#include "MCOHpWidget.h"
#include "MCOAttributeWidget.h"
#include "MCOSkillWidget.h"
#include "Interface/MCOHUDInterface.h"

UMCOHUDWidget::UMCOHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCOHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	IMCOHUDInterface* HUDPawn = Cast<IMCOHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetHUD(this);
	}
}

UMCOHpWidget* UMCOHUDWidget::GetHpWidget(bool bIsPlayer)
{
	FName CurName = GetName(bIsPlayer);
	ISTRUE_N(true == HpWidgets.Contains(CurName));
	return HpWidgets[CurName];
}

UMCOAttributeWidget* UMCOHUDWidget::GetAttributeWidget(bool bIsPlayer)
{
	FName CurName = GetName(bIsPlayer);
	ISTRUE_N(true == AttributeWidgets.Contains(CurName));
	return AttributeWidgets[CurName];
}

void UMCOHUDWidget::StartSkillWidget(UTexture2D* InImage, const float& InMax)
{
	// select idx...
	
	SkillWidgets[0]->StartSkillCooldown(InImage, InMax);
}

const FName UMCOHUDWidget::GetName(bool bIsPlayer) const
{
	return (bIsPlayer) ? TEXT("Player") : TEXT("Monster");
}

void UMCOHUDWidget::InitializeHUDWidget(bool bIsPlayer)
{
	FName CurName = GetName(bIsPlayer);
	
	HpWidgets.Emplace(CurName, Cast<UMCOHpWidget>(GetWidgetFromName(
		*FString::Printf(TEXT("WBP_%sHpBar"), *CurName.ToString())
	)));
	AttributeWidgets.Emplace(CurName, Cast<UMCOAttributeWidget>(GetWidgetFromName(
		*FString::Printf(TEXT("WBP_%sAttribute"), *CurName.ToString())
	)));

	for (int32 i = 0; i < SKILLSLOT_MAX; i++)
	{
		SkillWidgets.Emplace(Cast<UMCOSkillWidget>(GetWidgetFromName(
			*FString::Printf(TEXT("WBP_PlayerSkill_%d"), i + 1)
		)));
		
		ensure(SkillWidgets[i]);
	}
	
	ensure(HpWidgets[CurName]);
	ensure(AttributeWidgets[CurName]);
}

void UMCOHUDWidget::ShowWidget(bool bIsPlayer, bool bIsToShow)
{
	FName CurName = GetName(bIsPlayer);
	
	ISTRUE(true == HpWidgets.Contains(CurName));
	ISTRUE(nullptr != HpWidgets[CurName]);
	ISTRUE(true == AttributeWidgets.Contains(CurName));
	ISTRUE(nullptr != AttributeWidgets[CurName]);
	
	HpWidgets[CurName]->SetVisibility((true == bIsToShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	AttributeWidgets[CurName]->SetVisibility((true == bIsToShow) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
