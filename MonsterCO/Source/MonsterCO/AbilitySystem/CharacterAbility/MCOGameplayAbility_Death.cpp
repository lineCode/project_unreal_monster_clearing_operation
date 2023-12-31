#include "MCOGameplayAbility_Death.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOCharacterInterface.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"


UMCOGameplayAbility_Death::UMCOGameplayAbility_Death()
{	
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	
	DilationTime = 0.4f;
	DilationDuration = 1.0f;
}

// void UMCOGameplayAbility_Death::DoneAddingNativeTags()
// {	
// 	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_DeadTag);
// 	
// 	// Cancel ALL
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DodgeTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().AttackTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DamagedTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().TakeItemTag);
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
// }

void UMCOGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(false, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE(nullptr != CharacterInterface);

	if (false == IsPlayer())
	{
		SetDilation();
	}

	CharacterInterface->Die();
	
	CancelAllAbility();
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);
		
	FGameplayTagContainer Tags;
	Tags.AddTag(FMCOCharacterTags::Get().EffectRemoveOnDeathTag);
	ASC->RemoveActiveEffectsWithGrantedTags(Tags);

	const EMCOCharacterDirection Direction = Data->GetDirectionFromDegree(CharacterInterface->GetDamagedDegreeThenSetZero());
	
	StartActivationWithMontage(Data->GetMontage(Direction));
}

void UMCOGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE(nullptr != CharacterInterface);
	CharacterInterface->FinishDying();
}

void UMCOGameplayAbility_Death::SetDilation()
{
	ISTRUE(0.0f < DilationTime);
	ISTRUE(0.0f < DilationDuration);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), DilationTime);

	TimeDilationHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(
		TimeDilationHandle,
		this,
		&ThisClass::RestoreTimeDilation,
		DilationDuration,
		false
	);
}

void UMCOGameplayAbility_Death::RestoreTimeDilation()
{
	TimeDilationHandle.Invalidate();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
