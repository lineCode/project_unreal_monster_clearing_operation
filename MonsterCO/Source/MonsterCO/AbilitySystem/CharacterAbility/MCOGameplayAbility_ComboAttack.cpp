#include "MCOGameplayAbility_ComboAttack.h"
#include "AbilitySystem/ActionData/MCOMontageDataCombo.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Attribute.h"


UMCOGameplayAbility_ComboAttack::UMCOGameplayAbility_ComboAttack()
{
	GETASSET(Data, UMCOMontageDataCombo, TEXT("/Game/Data/Player/TwohandAction/DA_Twohand_Combo.DA_Twohand_Combo"));
	
	CurrentCombo = 1;
	Data->UpdateComboDefinition(CurrentDefinition);
}

// void UMCOGameplayAbility_ComboAttack::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	// Cancel these
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);
//
// 	// Blocked by these
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);
// }

void UMCOGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CurrentCombo = 1;
	bIsComboCommandPressed = false;
	bIsDoingCombo = false;
	ComboTimerHandle.Invalidate();
	
	ensure(nullptr != Data);
	Data->UpdateComboDefinition(CurrentDefinition);
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	StartActivation_CommonAttack(
		Data->GetMontage(CurrentCombo - 1),
		Data->MontageSectionName
	);

	SetComboTimer();
}

void UMCOGameplayAbility_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	bIsComboCommandPressed = true;
}

void UMCOGameplayAbility_ComboAttack::OnTaskCompleted()
{
	if(true == bIsDoingCombo)
	{
		bIsDoingCombo = false;
		return;
	}

	ComboTimerHandle.Invalidate();
	Super::OnTaskCompleted();
}

void UMCOGameplayAbility_ComboAttack::OnTaskCancelled()
{
	ComboTimerHandle.Invalidate();
	Super::OnTaskCancelled();
}

void UMCOGameplayAbility_ComboAttack::SetComboTimer()
{
	ComboTimerHandle.Invalidate();
	
	const float ComboCheckTime = CurrentDefinition->AttackTimingFragment->GetComboCheckTime();
	ISTRUE(ComboCheckTime > 0.0f);
	
	// MCOLOG_C(MCOAbility, TEXT("[Combo] SetComboTimer: %f"), ComboCheckTime);
	
	GetWorld()->GetTimerManager().SetTimer(
		ComboTimerHandle,
		this,
		&ThisClass::DoNextCombo,
		ComboCheckTime,
		false
	);
}

void UMCOGameplayAbility_ComboAttack::DoNextCombo()
{	
	ISTRUE(bIsComboCommandPressed == true);

	CurrentCombo++;
	bIsComboCommandPressed = false;

	int32 MaxCombo = Data->GetMaxCombo();
	// MCOLOG_C(MCOAbility, TEXT("[Combo] %d / %d"), CurrentCombo, MaxCombo);
	ISTRUE(CurrentCombo <= MaxCombo);

	bIsDoingCombo = true;

	Data->UpdateComboDefinition(CurrentDefinition, CurrentCombo - 1);
	
	if (false == CheckCanActivateWithStamina())
	{
		Data->UpdateComboDefinition(CurrentDefinition);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	// Play next Montage
	StartActivation_CommonAttack(
		Data->GetMontage(CurrentCombo - 1),
		Data->MontageSectionName
	);

	ApplyAttributeEffect(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	
	SetComboTimer();
}


