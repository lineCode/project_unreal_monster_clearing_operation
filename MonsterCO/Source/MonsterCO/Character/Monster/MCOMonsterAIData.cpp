#include "MCOMonsterAIData.h"


float UMCOMonsterAIData::GetAbilityRangeMax(const FGameplayTag& InTag)
{
	ensure(AbilityRanges.Contains(InTag));
	return AbilityRanges[InTag].End;
}

float UMCOMonsterAIData::GetAbilityRangeMin(const FGameplayTag& InTag)
{
	ensure(AbilityRanges.Contains(InTag));
	return AbilityRanges[InTag].Start;
}
