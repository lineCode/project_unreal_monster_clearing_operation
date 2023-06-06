#include "MCOGameplayAbility_Damaged.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"



UMCOGameplayAbility_Damaged::UMCOGameplayAbility_Damaged()
{
	SetID(EMCOAbilityID::Damaged, FMCOCharacterTags::Get().DamagedTag);
	SetTriggerTag(FMCOCharacterTags::Get().GameplayEvent_DamagedTag);
	
	// Cancel these 
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
}

void UMCOGameplayAbility_Damaged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));

	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	if (nullptr != PlayerInterface)
	{
		PlayerInterface->SetEquippedWithoutAnimation();
	}
	
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);
	
	CancelAllAbility();
	// CharacterInterface->OffAllCollision();
	CharacterInterface->OnDamagedBegin.ExecuteIfBound();
	StartActivationWithMontage(Data->GetMontage(CharacterInterface->GetDamagedData().AttackedDegree));
}

void UMCOGameplayAbility_Damaged::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	StartStaminaChargeTimer();
}

void UMCOGameplayAbility_Damaged::OnTaskCompleted()
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);
	CharacterInterface->OnDamagedEnd.ExecuteIfBound();
	
	Super::OnTaskCompleted();
}

void UMCOGameplayAbility_Damaged::OnTaskCancelled()
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(CurrentActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);
	CharacterInterface->OnDamagedEnd.ExecuteIfBound();
	
	Super::OnTaskCancelled();
}
