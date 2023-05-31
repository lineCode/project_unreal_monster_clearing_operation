#include "MCOAssetManager.h"
#include "AbilitySystemGlobals.h"

void UMCOAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
