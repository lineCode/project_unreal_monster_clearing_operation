#include "MCOGameplayAbility_ComboAttack.h"
#include "AbilitySystem/ActionData/MCOMontageDataCombo.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Montage.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Attribute.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"


UMCOGameplayAbility_ComboAttack::UMCOGameplayAbility_ComboAttack()
{
	GETASSET(Data, UMCOMontageDataCombo, TEXT("/Game/Data/Player/TwohandAction/DA_Twohand_Combo.DA_Twohand_Combo"));
	
	CurrentCombo = 1;
	ensure(nullptr != Data);
	const UMCOActionFragment_Montage* MontageFragment = Data->GetMontageFragment(CurrentCombo - 1);
	ensure(nullptr != MontageFragment);
	ensure(nullptr != MontageFragment->ActionDefinition);
	UpdateCooldownFragment(MontageFragment->ActionDefinition->GetCooldownFragment());
	const UMCOActionFragment_Attribute* Stamina = MontageFragment->ActionDefinition->GetAttributeFragment();
	UpdateAttributeFragment(Stamina);
	
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
	const UMCOActionFragment_Montage* MontageFragment = Data->GetMontageFragment(CurrentCombo - 1);
	ensure(nullptr != MontageFragment);
	ensure(nullptr != MontageFragment->ActionDefinition);
	UpdateCooldownFragment(MontageFragment->ActionDefinition->GetCooldownFragment());
	const UMCOActionFragment_Attribute* Stamina = MontageFragment->ActionDefinition->GetAttributeFragment();
	UpdateAttributeFragment(Stamina);
	
	
	ISTRUE(SetAndCommitAbility(true, Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	StartActivation_CommonAttack(
		MontageFragment->GetMontage(),
		Data->MontageSectionName,
		MontageFragment->ActionDefinition->GetTimerFragment(),
		MontageFragment->ActionDefinition->GetCollisionFragment()
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
	
	const float ComboCheckTime = TimerFragment->GetComboCheckTime();
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
	
	const UMCOActionFragment_Montage* MontageFragment = Data->GetMontageFragment(CurrentCombo - 1);
	ensure(MontageFragment);
	ensure(MontageFragment->ActionDefinition);

	const UMCOActionFragment_Attribute* CurrentAttributeFragment = MontageFragment->ActionDefinition->GetAttributeFragment();
	ensure(CurrentAttributeFragment);
	
	UpdateAttributeFragment(CurrentAttributeFragment);
	
	if (false == CheckCanActivateWithStamina())
	{
		UpdateAttributeFragment(Data->GetMontageFragment(0)->ActionDefinition->GetAttributeFragment());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	// Play next Montage
	StartActivation_CommonAttack(
		MontageFragment->GetMontage(),
		Data->MontageSectionName,
		MontageFragment->ActionDefinition->GetTimerFragment(),
		MontageFragment->ActionDefinition->GetCollisionFragment()
	);

	ApplyAttributeEffect(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	
	SetComboTimer();
}


