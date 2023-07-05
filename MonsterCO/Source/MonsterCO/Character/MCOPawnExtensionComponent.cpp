#include "Character/MCOPawnExtensionComponent.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"

UMCOPawnExtensionComponent::UMCOPawnExtensionComponent()
{
	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
}

void UMCOPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMCOPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	
}

UMCOPawnExtensionComponent* UMCOPawnExtensionComponent::FindPawnExtensionComponent(const AActor* Actor)
{
	return (nullptr != Actor) ? Actor->FindComponentByClass<UMCOPawnExtensionComponent>() : nullptr;
}

void UMCOPawnExtensionComponent::HandleControllerChanged()
{
	ISTRUE(nullptr != AbilitySystemComponent);
	ISTRUE(AbilitySystemComponent->GetAvatarActor() == GetOwner());

	ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
	
	if (nullptr == AbilitySystemComponent->GetOwnerActor())
	{
		UninitializeAbilitySystem();
	}
	else
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void UMCOPawnExtensionComponent::InitializeAbilitySystem(UMCOAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	ensure(nullptr != InASC);
	ensure(nullptr != InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		// The ability system component hasn't changed.
		return;
	}

	if (nullptr != AbilitySystemComponent)
	{
		// Clean up the old ability system component.
		UninitializeAbilitySystem();
	}

	APawn* Pawn = GetOwner<APawn>();
	AActor* ExistingAvatar = InASC->GetAvatarActor();

	MCOLOG_C(MCOAbility, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(InASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));

	if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	{
		MCOLOG_C(MCOAbility, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);

		// There is already a pawn acting as the ASC's avatar, so we need to kick it out
		// This can happen on clients if they're lagged: their new pawn is spawned + possessed before the dead one is removed
		ensure(false == ExistingAvatar->HasAuthority());

		if (UMCOPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
		{
			OtherExtensionComponent->UninitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
	AbilitySystemComponent->SetTagMapCount(FMCOCharacterTags::Get().DeadTag, 0);
}

void UMCOPawnExtensionComponent::UninitializeAbilitySystem()
{
	ISTRUE(nullptr != AbilitySystemComponent.Get());
	
	// Uninitialize the ASC if we're still the avatar actor
	// (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (nullptr != AbilitySystemComponent->GetOwnerActor())
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner,
			// we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}
	}
	
	AbilitySystemComponent = nullptr;
}


