#include "MCOPlayerState.h"
#include "Player/MCOPlayerCharacter.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOCharacterTags.h"


AMCOPlayerState::AMCOPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UMCOAbilitySystemComponent>(TEXT("NAME_AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    NetUpdateFrequency = 100.0f;
}

void AMCOPlayerState::InitializeAbilitySystem()
{
    ISTRUE(nullptr != AbilitySystemComponent);

    // attribute set after giving abilities in character 
    AttributeSet = AbilitySystemComponent->GetSet<UMCOAttributeSet>();
    AbilitySystemComponent->AttributeSet = AttributeSet;

    // delegates
    OnAttributeChangedDelegate.Empty();
    AttributeSet->OnHandleAttributeEventDelegate.AddUObject(this, &ThisClass::HandleEventWithTag);
    
    RegisterAttributeChangedDelegate(AttributeSet->GetHealthAttribute());
    RegisterAttributeChangedDelegate(AttributeSet->GetMaxHealthAttribute());
    RegisterAttributeChangedDelegate(AttributeSet->GetStaminaAttribute());
    RegisterAttributeChangedDelegate(AttributeSet->GetMaxStaminaAttribute());
    RegisterAttributeChangedDelegate(AttributeSet->GetStiffnessAttribute());
    RegisterAttributeChangedDelegate(AttributeSet->GetMaxStiffnessAttribute());

}

void AMCOPlayerState::BeginPlay()
{
    Super::BeginPlay();
}

void AMCOPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    check(nullptr != AbilitySystemComponent);
    AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}

UMCOAbilitySystemComponent* AMCOPlayerState::GetMCOAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAbilitySystemComponent* AMCOPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

bool AMCOPlayerState::IsAlive() const
{
    return AttributeSet->GetHealth() > 0.0f;
}

void AMCOPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
    // TODO : implement HUD later
}

void AMCOPlayerState::RegisterAttributeChangedDelegate(const FGameplayAttribute& Attribute) 
{
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        Attribute
    ).AddUObject(this, &AMCOPlayerState::OnAttributeChanged);
}

void AMCOPlayerState::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
    //MCOPRINT(TEXT("Attribute changed : [%s] -> [%f]"), *Data.Attribute.GetName(), Data.NewValue);
    
    FName CurName = *Data.Attribute.GetName();
    ISTRUE(true == OnAttributeChangedDelegate.Contains(CurName));
    ISTRUE(true == OnAttributeChangedDelegate[CurName].AttributeDelegate.IsBound());
   
    OnAttributeChangedDelegate[CurName].AttributeDelegate.Broadcast(Data.NewValue);
}

void AMCOPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    ISTRUE(NewCount > 0);
    
    MCOPRINT(TEXT("StunTag changed! And cancelled some abilities"));

	FGameplayTagContainer AbilityTagsToCancel;
	AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("State")));

	FGameplayTagContainer AbilityTagsToIgnore;
	// AbilityTagsToIgnore.AddTag();

	AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
    
    ISTRUE(true == OnTagChangedDelegate.IsBound());
    OnTagChangedDelegate.Broadcast(FMCOCharacterTags::Get().DashTag, (0 < NewCount));
}

void AMCOPlayerState::HandleEventWithTag(const FGameplayTag& InTag, AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& EffectSpec, float Magnitude) const
{
    ISTRUE(nullptr != AbilitySystemComponent);
    
    // Send the "InTag" gameplay event through the owner's ability system.
    // This can be used to trigger a death gameplay ability.
    FGameplayEventData Payload;
    Payload.EventTag       = InTag;
    Payload.Instigator     = DamageInstigator;
    Payload.Target         = AbilitySystemComponent->GetAvatarActor();
    Payload.OptionalObject = EffectSpec.Def;
    Payload.ContextHandle  = EffectSpec.GetEffectContext();
    Payload.InstigatorTags = *EffectSpec.CapturedSourceTags.GetAggregatedTags();
    Payload.TargetTags     = *EffectSpec.CapturedTargetTags.GetAggregatedTags();
    Payload.EventMagnitude = Magnitude;

    FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
    AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
}
