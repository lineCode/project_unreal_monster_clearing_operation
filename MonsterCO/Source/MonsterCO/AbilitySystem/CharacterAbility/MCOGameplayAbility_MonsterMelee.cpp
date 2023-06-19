#include "MCOGameplayAbility_MonsterMelee.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"


UMCOGameplayAbility_MonsterMelee::UMCOGameplayAbility_MonsterMelee()
{
	GETASSET(Data, UMCOMontageDataDirectional, TEXT("/Game/Data/Monster/Dragon/Action/DA_Dragon_Melee.DA_Dragon_Melee"));
}

void UMCOGameplayAbility_MonsterMelee::SetDefaultDefinition()
{
	ensure(nullptr != Data);
	Data->UpdateDirectionalDefinition(CurrentDefinition);
}

bool UMCOGameplayAbility_MonsterMelee::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (false == bResult)
	{
		const IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(ActorInfo->AvatarActor.Get());
		ensure(MonsterInterface);
		MonsterInterface->OnAttackFinished.ExecuteIfBound(EBTNodeResult::Failed);
	}
	
	return bResult;
}

void UMCOGameplayAbility_MonsterMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(nullptr != ActorInfo);
	
	AActor* Monster = ActorInfo->AvatarActor.Get();
	ensure(Monster);

	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(Monster);
	ensure(MonsterInterface);
	
	AActor* Target = Cast<AActor>(MonsterInterface->GetTarget());
	ISTRUE(Target);
	
	
	const FVector Source = FVector(Monster->GetActorLocation().X, Monster->GetActorLocation().Y, 0.0f);
	const FVector Dest = FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.0f);
	const FVector LookVector = (Dest - Source).GetSafeNormal();
	
	const float AttackDegree = CalculateDegree(
		Monster->GetActorLocation(),
		Monster->GetActorForwardVector(),
		LookVector
	);
	
	ensure(nullptr != Data);

	const EMCOCharacterDirection Direction = Data->GetDirectionFromDegree(AttackDegree);
	Data->UpdateDirectionalDefinition(CurrentDefinition, Direction);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	StartActivation_CommonAttack(
		Data->GetMontage(Direction),
		Data->MontageSectionName
	);
}

void UMCOGameplayAbility_MonsterMelee::OnTaskCompleted()
{
	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(MonsterInterface);
	MonsterInterface->OnAttackFinished.ExecuteIfBound(EBTNodeResult::Succeeded);
	
	Super::OnTaskCompleted();
}

void UMCOGameplayAbility_MonsterMelee::OnTaskCancelled()
{
	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(MonsterInterface);
	MonsterInterface->OnAttackFinished.ExecuteIfBound(EBTNodeResult::Failed);
	
	Super::OnTaskCancelled();
}