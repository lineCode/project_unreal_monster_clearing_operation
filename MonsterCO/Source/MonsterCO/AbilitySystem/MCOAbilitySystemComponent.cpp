#include "MCOAbilitySystemComponent.h"
#include "MCOAttributeSet.h"
#include "MCOCharacterTags.h"
#include "CharacterAbility/MCOGameplayAbility.h"


UMCOAbilitySystemComponent::UMCOAbilitySystemComponent()
{
	bCharacterAbilitySetGiven = false;
}

void UMCOAbilitySystemComponent::ReceiveDamage(UMCOAbilitySystemComponent* SourceASC, float Damage, float TotalStiffness)
{
	OnDamagedReceived.Broadcast(SourceASC, Damage);
}

void UMCOAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& InTag)
{
	ISTRUE(true == InTag.IsValid());
	
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability == nullptr) continue;
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InTag) == false) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UMCOAbilitySystemComponent::CancelAbilityByTag(const FGameplayTag& InTag)
{
	ISTRUE(true == InTag.IsValid());

	FGameplayTagContainer Tags;
	Tags.AddTag(InTag);
	CancelAbilities(&Tags);
}

void UMCOAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	ISTRUE(true == InputTag.IsValid());
	
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability == nullptr) continue;
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) == false) continue;

		// MCOLOG_C(MCOAbility, TEXT("[%s] is pressed"), *InputTag.GetTagName().ToString());

		InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
		InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
	}
}

void UMCOAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	ISTRUE(InputTag.IsValid() == true);
	
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability == nullptr) continue;
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) == false) continue;

		//MCOLOG_C(MCOAbility, TEXT("[%s] is released"), *InputTag.GetTagName().ToString());
		
		InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
		InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
	}
}

void UMCOAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();
	
	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
		if (AbilitySpec == nullptr || AbilitySpec->Ability == nullptr || AbilitySpec->IsActive() == true) continue;
	
		const UMCOGameplayAbility* MCOAbilityCDO = CastChecked<UMCOGameplayAbility>(AbilitySpec->Ability);
		if (MCOAbilityCDO->GetActivationPolicy() != EMCOAbilityActivationPolicy::WhileInputActive) continue;
		
		AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
	}
	
	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
		if (AbilitySpec->Ability == nullptr) continue;
		
		AbilitySpec->InputPressed = true;
	
		if (AbilitySpec->IsActive() == true)
		{
			// Ability is active so pass along the input event.
			AbilitySpecInputPressed(*AbilitySpec);
		}
		else
		{
			const UMCOGameplayAbility* MCOAbilityCDO = CastChecked<UMCOGameplayAbility>(AbilitySpec->Ability);
			if (MCOAbilityCDO->GetActivationPolicy() != EMCOAbilityActivationPolicy::OnInputTriggered) continue;
	
			AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
		}
	}
	
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	
	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
		if (AbilitySpec->Ability == nullptr) continue;
		
		AbilitySpec->InputPressed = false;
		if (AbilitySpec->IsActive() == false) continue;
		
		// Ability is active so pass along the input event.
		AbilitySpecInputReleased(*AbilitySpec);
	}

	// Clear the cached ability handles.
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UMCOAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

