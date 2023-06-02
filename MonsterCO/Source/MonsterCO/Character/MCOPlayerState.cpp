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

void AMCOPlayerState::InitializeAbilityDelegates()
{
    ISTRUE(nullptr != AbilitySystemComponent);

    AttributeSet = AbilitySystemComponent->GetSet<UMCOAttributeSet>();
    AttributeSet->OnOutOfHealthDelegate.AddUObject(this, &ThisClass::HandleOutOfHealth);
    AttributeSet->OnOutOfStiffnessDelegate.AddUObject(this, &ThisClass::HandleOutOfStiffness);
    AbilitySystemComponent->AttributeSet = AttributeSet;
    
    // connect Attributes in AttributeSet to Changed Functions in PlayerState
    // if Attributes change, Changed Functions here will be called
    HealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AttributeSet->GetHealthAttribute()
    ).AddUObject(this, &AMCOPlayerState::OnAttributeChanged);
    
    MaxHealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AttributeSet->GetMaxHealthAttribute()
    ).AddUObject(this, &AMCOPlayerState::OnAttributeChanged);
    
    StiffnessChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AttributeSet->GetStiffnessAttribute()
    ).AddUObject(this, &AMCOPlayerState::OnAttributeChanged);

    MaxStiffnessChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AttributeSet->GetMaxStiffnessAttribute()
    ).AddUObject(this, &AMCOPlayerState::OnAttributeChanged);
    
    // connect GameplayTag to Changed Function in PlayerState
    AbilitySystemComponent->RegisterGameplayTagEvent(
        FGameplayTag::RequestGameplayTag((FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved)
    ).AddUObject(this, &AMCOPlayerState::StunTagChanged);
}

void AMCOPlayerState::BeginPlay()
{
    Super::BeginPlay();
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

void AMCOPlayerState::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
    //MCOPRINT(TEXT("%s is changed! : %f"), *Data.Attribute.GetName(), Data.NewValue);
    
    FName CurName = *Data.Attribute.GetName();
    ISTRUE(true == OnAttributeChangedDelegate.Contains(CurName));
    ISTRUE(true == OnAttributeChangedDelegate[CurName].AttributeDelegate.IsBound());
   
    OnAttributeChangedDelegate[CurName].AttributeDelegate.Broadcast(Data.NewValue);
}

void AMCOPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    MCOPRINT(TEXT("StunTag changed! And cancelled some abilities"));
    
    ISTRUE(NewCount > 0);

	FGameplayTagContainer AbilityTagsToCancel;
	AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	FGameplayTagContainer AbilityTagsToIgnore;
	AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCancelByStun")));

	AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
}

void AMCOPlayerState::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
    HandleEventWithTag(FMCOCharacterTags::Get().GameplayEvent_DeadTag, DamageInstigator, DamageCauser, DamageEffectSpec, DamageMagnitude);
}

void AMCOPlayerState::HandleOutOfStiffness(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
    HandleEventWithTag(FMCOCharacterTags::Get().GameplayEvent_DamagedTag, DamageInstigator, DamageCauser, DamageEffectSpec, DamageMagnitude);
}

void AMCOPlayerState::HandleEventWithTag(const FGameplayTag& InTag, AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude) const
{
    ISTRUE(nullptr != AbilitySystemComponent);
    
    // Send the "InTag" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
    FGameplayEventData Payload;
    Payload.EventTag = InTag;
    Payload.Instigator = DamageInstigator;
    Payload.Target = AbilitySystemComponent->GetAvatarActor();
    Payload.OptionalObject = DamageEffectSpec.Def;
    Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
    Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
    Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
    Payload.EventMagnitude = DamageMagnitude;

    FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
    AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
}
