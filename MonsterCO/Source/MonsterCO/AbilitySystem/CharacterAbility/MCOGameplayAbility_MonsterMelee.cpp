#include "MCOGameplayAbility_MonsterMelee.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Cooldown.h"


UMCOGameplayAbility_MonsterMelee::UMCOGameplayAbility_MonsterMelee()
{
	GETASSET(Data, UMCOMontageDataDirectional, TEXT("/Game/Data/Monster/Dragon/Action/DA_Dragon_Claw.DA_Dragon_Claw"));

	SetID(EMCOAbilityID::NormalAttack, Data->ActivationTag);
	
	ensure(nullptr != Data->ActionDefinition);
	const UMCOActionFragment_Cooldown* Fragment = Data->ActionDefinition->GetCooldownFragment();
	ensure(nullptr != Fragment);
	UpdateCooldownFragment(Fragment);
}

bool UMCOGameplayAbility_MonsterMelee::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
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

	const float AttackDegree = CalculateTargetDegree(
		Monster->GetActorLocation(),
		Monster->GetActorForwardVector(),
		Target->GetActorLocation()
	);
	
	ensure(nullptr != Data);
	ensure(nullptr != Data->ActionDefinition);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	StartActivation_CommonAttack(
		Data->GetMontage(AttackDegree),
		Data->MontageSectionName,
		Data->ActionDefinition->GetTimerFragment(), 
		Data->ActionDefinition->GetDamageFragment(),
		Data->GetCollisionFragment(AttackDegree)
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

void UMCOGameplayAbility_MonsterMelee::BeginDamaging_Collision()
{
	Super::BeginDamaging_Collision();

	ISTRUE(nullptr != Data);
	
	IMCOMonsterAIInterface* Monster = Cast<IMCOMonsterAIInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(Monster);
	
	// UMCOMonsterModeComponent* Mode = Monster->GetModeComponent();
	// ensure(Mode);
	//
	// AMCOMonsterAttachment* Attachment = Mode->GetAttachment();
	// ensure(Attachment);
	//
	// Attachment->OnAttachmentBeginOverlapDelegate.AddUniqueDynamic(this, &ThisClass::OnAttachmentBeginOverlap);
	// Attachment->TurnOnCollision(CurrentData->CollisionData.SocketName);
}

void UMCOGameplayAbility_MonsterMelee::EndDamaging_Collision()
{
	Super::EndDamaging_Collision();
	
	ISTRUE(nullptr != Data);
	
	IMCOMonsterAIInterface* Monster = Cast<IMCOMonsterAIInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(Monster);
	
	// UMCOMonsterModeComponent* Mode = Monster->GetModeComponent();
	// ensure(Mode);
	//
	// AMCOMonsterAttachment* Attachment = Mode->GetAttachment();
	// ensure(Attachment);
	//
	// Attachment->OnAttachmentBeginOverlapDelegate.Clear();
	// Attachment->TurnOffCollision(CurrentData->CollisionData.SocketName);
}

