#include "MCOGameplayAbility_ComboAttack.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "Interface/MCOCharacterInterface.h"
#include "AbilitySystem/ActionData/MCOMontageDataCombo.h"
#include "AbilitySystem/ActionData/MCOMontageDataUnit.h"
#include "AbilitySystem/ActionData/MCOAttackFragment_Timer.h"


UMCOGameplayAbility_ComboAttack::UMCOGameplayAbility_ComboAttack()
{
	AbilityInputID = EMCOAbilityID::NormalAttack;
	AbilityTag = FMCOCharacterTags::Get().AttackTag;
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	
	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().DashTag);

	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().EquipTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().JumpTag);
	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DodgeTag);

	// Set Data...
}

bool UMCOGameplayAbility_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ISTRUE_F(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));

	const IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(ActorInfo->AvatarActor.Get());
	ISTRUE_F(CharacterInterface);
	ISTRUE_F(CharacterInterface->CanAttack());

	return true;
}

void UMCOGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CurrentCombo = 1;
	bIsComboCommandPressed = false;
	bIsDoingCombo = false;
	ComboTimerHandle.Invalidate();
	
	ensure(nullptr != Data);
	UMCOMontageDataUnit* DataUnit = Data->GetMontageData(CurrentCombo - 1);
	ensure(DataUnit);
	
	// Set Cooldown Effect
	SetCooldownFragment(DataUnit->CooldownFragment);
	
	ISTRUE(SetAndCommitAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData));
	
	StartActivation_CommonAttack(
		DataUnit->GetMontage(),
		Data->MontageSectionName,
		DataUnit->TimerFragment,
		DataUnit->DamageFragment,
		DataUnit->CollisionFragment
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
	if(true == bIsDoingCombo)
	{
		bIsDoingCombo = false;
		return;
	}
	
	MCOLOG(TEXT("Combo Cancelled"));
	ComboTimerHandle.Invalidate();
	Super::OnTaskCancelled();
}

void UMCOGameplayAbility_ComboAttack::BeginDamaging_Collision()
{
	Super::BeginDamaging_Collision();

	//AMCOPlayerCharacter* Owner = Cast<AMCOPlayerCharacter>(GetMCOCharacter());
	//ensure(Owner);
	
	//AMCOWeapon* Weapon = Owner->GetWeapon();
	//ensure(Weapon);
	
	//Weapon->OnAttachmentBeginOverlapDelegate.AddUniqueDynamic(this, &ThisClass::OnAttachmentBeginOverlap);
	//Weapon->TurnOnAllCollision();
}

void UMCOGameplayAbility_ComboAttack::EndDamaging_Collision()
{
	Super::EndDamaging_Collision();

	//AMCOPlayerCharacter* Owner = Cast<AMCOPlayerCharacter>(GetMCOCharacter());
	//ensure(Owner);
	
	//AMCOWeapon* Weapon = Owner->GetWeapon();
	//ensure(Weapon);
	
	//Weapon->OnAttachmentBeginOverlapDelegate.Clear();
	//Weapon->TurnOffAllCollision();
}

void UMCOGameplayAbility_ComboAttack::SetComboTimer()
{
	ComboTimerHandle.Invalidate();
	
	float ComboCheckTime = TimerFragment->GetComboCheckTime();
	ISTRUE(ComboCheckTime > 0.0f);
	
	// MCOLOG(TEXT("[Combo] SetComboTimer: %f"), ComboCheckTime);
	
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
	// MCOLOG(TEXT("[Combo] %d / %d"), CurrentCombo, MaxCombo);
	ISTRUE(CurrentCombo <= MaxCombo);

	bIsDoingCombo = true;
	
	UMCOMontageDataUnit* DataUnit = Data->GetMontageData(CurrentCombo - 1);
	ensure(DataUnit);
	
	// Play next Montage
	StartActivation_CommonAttack(
		DataUnit->GetMontage(),
		Data->MontageSectionName,
		DataUnit->TimerFragment,
		DataUnit->DamageFragment,
		DataUnit->CollisionFragment
	);
		
	SetComboTimer();
}


