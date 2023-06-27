#include "MCOGA_MonsterDirectionalAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"


UMCOGA_MonsterDirectionalAttack::UMCOGA_MonsterDirectionalAttack()
{
	GETASSET(Data, UMCOMontageDataDirectional, TEXT("/Game/Data/Monster/Dragon/Action/DA_Dragon_Claw.DA_Dragon_Claw"));
}

void UMCOGA_MonsterDirectionalAttack::SetDefaultDefinition()
{
	Super::SetDefaultDefinition();
	
	ensure(nullptr != Data);
	Data->UpdateDirectionalDefinition(CurrentDefinition);
}

void UMCOGA_MonsterDirectionalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(nullptr != ActorInfo);
	
	AActor* Monster = ActorInfo->AvatarActor.Get();
	ensure(nullptr != Monster);

	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(Monster);
	ensure(nullptr != MonsterInterface);
	
	const AActor* Target = Cast<AActor>(MonsterInterface->GetTarget());
	ISTRUE(nullptr != Target);
	
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
	
	StartAttackActivation(
		Data->GetMontage(Direction),
		Data->MontageSectionName
	);
}
