#include "MCOAbilitySet.h"
#include "GameplayAbilitySpec.h"
#include "MCOAbilitySystemComponent.h"
#include "CharacterAbility/MCOGameplayAbility.h"



void FMCOAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FMCOAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FMCOAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FMCOAbilitySet_GrantedHandles::TakeFromAbilitySystem(UMCOAbilitySystemComponent* MCOASC)
{
	check(MCOASC);

	if (false == MCOASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (int32 i = AbilitySpecHandles.Num() - 1; i >= 0; i--)
	{
		if (AbilitySpecHandles.Num() <= i)
		{
			break;
		}
		
		if (AbilitySpecHandles[i].IsValid())
		{
			MCOASC->ClearAbility(AbilitySpecHandles[i]);
		}
	}

	for (int32 i = GameplayEffectHandles.Num() - 1; i >= 0; i--)
	{
		if (GameplayEffectHandles.Num() <= i)
		{
			break;
		}
		
		if (GameplayEffectHandles[i].IsValid())
		{
			MCOASC->RemoveActiveGameplayEffect(GameplayEffectHandles[i]);
		}
	}

	for (int32 i = GrantedAttributeSets.Num() - 1; i >= 0; i--)
	{
		if (GrantedAttributeSets.Num() <= i)
		{
			break;
		}
		
		MCOASC->RemoveSpawnedAttribute(GrantedAttributeSets[i]);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
	
	MCOASC->bCharacterAbilitySetGiven = false;
}




UMCOAbilitySet::UMCOAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMCOAbilitySet::GiveToAbilitySystem(UMCOAbilitySystemComponent* MCOASC, FMCOAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	if (true == MCOASC->bCharacterAbilitySetGiven)
	{
		return;
	}
	
	check(MCOASC);
	
	if (false == MCOASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FMCOAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (false == IsValid(AbilityToGrant.Ability))
		{
			MCOLOG(TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UMCOGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UMCOGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = MCOASC->GiveAbility(AbilitySpec);
		
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FMCOAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (false == IsValid(SetToGrant.AttributeSet))
		{
			MCOLOG(TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(MCOASC->GetOwner(), SetToGrant.AttributeSet);
		MCOASC->AddAttributeSetSubobject(NewSet);
		
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
	
	AddStartupEffects(MCOASC, OutGrantedHandles, SourceObject);
	
	MCOASC->bCharacterAbilitySetGiven = true;
}

void UMCOAbilitySet::AddStartupEffects(UMCOAbilitySystemComponent* MCOASC, FMCOAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(MCOASC);
	
	if (false == MCOASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}
	
	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FMCOAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (false == IsValid(EffectToGrant.GameplayEffect))
		{
			MCOLOG(TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}
		
		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = MCOASC->ApplyGameplayEffectToSelf(
			GameplayEffect,
			EffectToGrant.EffectLevel,
			MCOASC->MakeEffectContext()
		);
		
		if (OutGrantedHandles)
		{
			// GameplayEffectHandle will be -1 if it's an instant effect (because it's finished)
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
