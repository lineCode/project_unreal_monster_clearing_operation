#include "AbilitySystem/CharacterAbility/AttackAbility/MCOGA_MonsterSignalAttack.h"
#include "AbilitySystem/ActionData/MCOActionData.h"
#include "AbilitySystem/ActionData/MCOActionFragment_MonsterAI.h"

UMCOGA_MonsterSignalAttack::UMCOGA_MonsterSignalAttack()
{
	GETASSET(Data, UMCOActionData, TEXT("/Game/Data/Monster/Dragon/Action/DA_Dragon_Falldown.DA_Dragon_Falldown"));
	
}

void UMCOGA_MonsterSignalAttack::SetDefaultDefinition()
{
	Super::SetDefaultDefinition();
	
	ensure(nullptr != Data);
	Data->UpdateDefinition(CurrentDefinition);
}

void UMCOGA_MonsterSignalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ensure(nullptr != Data);
	ensure(nullptr != MonsterAIFragment);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	FMCOActivateActionDelegate ActivateActionDelegate;
	ActivateActionDelegate.BindLambda([&]()
	{
		bIsInstantAttack = true;
		
		StartAttackActivation(
			nullptr,
			NAME_None
		);
	});

	MonsterAIFragment->OnActionFinished(CurrentActorInfo->AvatarActor.Get(), true);
	
	MonsterAIFragment->SetActivateActionDelegate(CurrentActorInfo->AvatarActor.Get(), ActivateActionDelegate);
}
