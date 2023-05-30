#include "MCOGameplayAbility_Damaged.h"
#include "MCOAbilitySystemComponent.h"
#include "MCOAttributeSet.h"
#include "MCOCharacterTags.h"
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

	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ensure(CharacterInterface);
	
	// Using Montage
	const EMCOCharacterDirection DamagedDirection = GetDirectionFromDegree(DirectionOption, CharacterInterface->GetDamagedData().AttackedDegree);
	ensure(Montages.Contains(DamagedDirection));
	
	MCOLOG(TEXT("Damaged %f : %s"),
		CharacterInterface->GetDamagedData().AttackedDegree,
		*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)DamagedDirection)
	);

	IMCOPlayerInterface* PlayerInterface = Cast<IMCOPlayerInterface>(ActorInfo->AvatarActor.Get());
	if (nullptr != PlayerInterface)
	{
		PlayerInterface->SetEquippedWithoutAnimation();
	}
	
	CancelAllAbility();
	// CharacterInterface->OffAllCollision();
	CharacterInterface->OnDamagedBegin.ExecuteIfBound();
	StartActivationWithMontage(Montages[DamagedDirection]);
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
