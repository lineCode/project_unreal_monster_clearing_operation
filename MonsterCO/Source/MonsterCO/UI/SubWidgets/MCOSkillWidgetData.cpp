#include "MCOSkillWidgetData.h"

void UMCOSkillWidgetData::SetData(const FGameplayTag& InTag, const uint8 InSlotLocation, const FText& InKeyText)
{
	if(false == SkillUIDatas.Contains(InTag))
	{
		SkillUIDatas.Emplace(InTag, FSkillUIData());
	}

	SkillUIDatas[InTag].SlotLocation = InSlotLocation;
	SkillUIDatas[InTag].KeyText = InKeyText;
}

UTexture2D* UMCOSkillWidgetData::GetTexture(const FGameplayTag& InTag)
{
	if(false == SkillUIDatas.Contains(InTag))
	{
		return nullptr;
	}
	
	return SkillUIDatas[InTag].SkillImage;
}

int8 UMCOSkillWidgetData::GetIndex(const FGameplayTag& InTag)
{
	if (false == SkillUIDatas.Contains(InTag))
	{
		return -1;
	}
	return SkillUIDatas[InTag].SlotLocation;
}
