#include "AbilitySystem/CharacterAbility/MCOGameplayAbility_SingleAttack.h"
#include "AbilitySystem/ActionData/MCOMontageDataSingle.h"


UMCOGameplayAbility_SingleAttack::UMCOGameplayAbility_SingleAttack()
{
	GETASSET(Data, UMCOMontageDataSingle, TEXT("/Game/Data/Player/TwohandAction/DA_Twohand_WhirlWind.DA_Twohand_WhirlWind"));
}

void UMCOGameplayAbility_SingleAttack::SetDefaultDefinition()
{
	ensure(nullptr != Data);
	Data->UpdateDefinition(CurrentDefinition);
}

void UMCOGameplayAbility_SingleAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ensure(nullptr != Data);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	StartActivation_CommonAttack(
		Data->Montage,
		Data->MontageSectionName
	);
}
