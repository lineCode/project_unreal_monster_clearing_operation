#include "MCOAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/MCOCharacterTags.h"

void UMCOAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();

	FMCOCharacterTags::InitializeTags();
}
