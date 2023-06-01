#include "MCOGameplayAbility_Damaged.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/ActionData/MCOMontageDataDirectional.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOPlayerInterface.h"



UMCOGameplayAbility_Damaged::UMCOGameplayAbility_Damaged()
{
	AbilityInputID = EMCOAbilityID::Damaged;
	AbilityTag = FMCOCharacterTags::Get().DamagedTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
		
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FMCOCharacterTags::Get().GameplayEvent_DamagedTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Emplace(TriggerData);

	// set direction option & data...
}

void UMCOGameplayAbility_Damaged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));

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
