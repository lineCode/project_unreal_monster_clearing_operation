#include "MCOActionFragment_Cooldown.h"
#include "GameplayEffect.h"
#include "AbilitySystem/MCOCharacterTags.h"

bool UMCOActionFragment_Cooldown::CanApplyCooldown() const
{
	return CooldownTime > 0.0f && CooldownTags.IsEmpty() == false;
}

void UMCOActionFragment_Cooldown::ApplyCooldownValue(const FGameplayEffectSpecHandle& InHandle) const
{
	InHandle.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_DurationTag,
		CooldownTime
	);
	
	InHandle.Data->DynamicGrantedTags = CooldownTags;
	InHandle.Data->DynamicGrantedTags.AddTag(FMCOCharacterTags::Get().EffectRemoveOnDeathTag);

	//MCOLOG_C(MCOAbility, TEXT("Cooldown Effect : %s [%.1f]sec"), *CooldownTags.First().GetTagName().ToString(), CooldownTime);
}

const FGameplayTagContainer* UMCOActionFragment_Cooldown::GetCooldownTags() const
{
	return &CooldownTags;
}
