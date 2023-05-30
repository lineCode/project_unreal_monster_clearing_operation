#include "MCOGameplayAbility_MonsterMelee.h"
#include "AbilitySystemComponent.h"
#include "Ability_Character/MCOCharacterTags.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "MonsterAttachment/MCOMonsterAttachment.h"
#include "MonsterAttachment/MCOMonsterModeComponent.h"


UMCOGameplayAbility_MonsterMelee::UMCOGameplayAbility_MonsterMelee()
{
	AbilityInputID = EMCOAbilityID::NormalAttack;
	AbilityTag = FMCOCharacterTags::Get().AttackTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);

	static ConstructorHelpers::FObjectFinder<UMCOCommonMontageData> LeftRef(TEXT("/Game/Data/Dragon/DA_LeftClaw.DA_LeftClaw"));
	if (true == LeftRef.Succeeded())
	{
		Data.Emplace(EMCOCharacterDirection::Left, LeftRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMCOCommonMontageData> RightRef(TEXT("/Game/Data/Dragon/DA_RightClaw.DA_RightClaw"));
	if (true == RightRef.Succeeded())
	{
		Data.Emplace(EMCOCharacterDirection::Right, RightRef.Object);
	}

	DirectionOption = EMCOCharacterDirectionOption::LeftRight;
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
	const EMCOCharacterDirection AttackingDirection = GetDirectionFromDegree(DirectionOption, AttackDegree);
	ensure(Data.Contains(AttackingDirection));
	
	// Set Cooldown Effect
	SetCooldownGameplayEffect(Data[AttackingDirection]->CommonSkillData);
	
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	// MCOLOG(TEXT("Monster Attack %f : %s"),
	// 	AttackDegree,
	// 	*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)AttackingDirection)
	// );
	
	StartActivation_CommonAttack(
		Data[AttackingDirection]
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
	MCOLOG(TEXT("Attack Cancelled"));
	IMCOMonsterAIInterface* MonsterInterface = Cast<IMCOMonsterAIInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(MonsterInterface);
	MonsterInterface->OnAttackFinished.ExecuteIfBound(EBTNodeResult::Failed);
	
	Super::OnTaskCancelled();
}

void UMCOGameplayAbility_MonsterMelee::BeginDamaging_Collision()
{
	Super::BeginDamaging_Collision();

	ISTRUE(nullptr != CurrentData);
	
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
	
	ISTRUE(nullptr != CurrentData);
	
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

