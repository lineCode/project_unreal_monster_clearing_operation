#include "UI/MCOSkillWidgetData.h"

void UMCOSkillWidgetData::SetData(const FGameplayTag& InTag, const uint8 InSlotLocation, const FText& InKeyText)
{
	ensure(true == SkillUIDatas.Contains(InTag));

	SkillUIDatas[InTag].SlotLocation = InSlotLocation;
	SkillUIDatas[InTag].KeyText = InKeyText;
}

UTexture2D* UMCOSkillWidgetData::GetTexture(const FGameplayTag& InTag)
{
	ensure(true == SkillUIDatas.Contains(InTag));
	return SkillUIDatas[InTag].SkillImage;
}

int8 UMCOSkillWidgetData::GetIndex(const FGameplayTag& InTag)
{
	if (true == SkillUIDatas.Contains(InTag))
	{
		return SkillUIDatas[InTag].SlotLocation;
	}
	return -1;
}
