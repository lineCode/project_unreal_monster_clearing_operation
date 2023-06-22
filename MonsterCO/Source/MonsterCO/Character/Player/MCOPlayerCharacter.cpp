#include "MCOPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/MCOInputComponent.h"
#include "Input/MCOInputConfig.h"
#include "MCOPlayerControlData.h"
#include "MCOPlayerSetting.h"
#include "Character/MCOCharacterData.h"
#include "Character/MCOPlayerState.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "CharacterAttachment/Attachment/MCOAttachment.h"
#include "CharacterAttachment/MCOPlayerModeComponent.h"
#include "CharacterAttachment/Attachment/MCOWeapon.h"
#include "UI/MCOHUDWidget.h"
#include "UI/MCOWidgetComponent.h"
#include "UI/Widgets/MCOStaminaWidget.h"
#include "Interface/MCOGameModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Item/MCOItemData_Potion.h"


AMCOPlayerCharacter::AMCOPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GETASSET(CharacterControlData, UMCOPlayerControlData, TEXT("/Game/Data/Player/DA_PlayerControl.DA_PlayerControl"));
	GETASSET(InputConfig, UMCOInputConfig, TEXT("/Game/Input/ABC_InputActions.ABC_InputActions"));
	GETASSET(CharacterData, UMCOCharacterData, TEXT("/Game/Data/Player/DA_Player.DA_Player"));
		
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("NAME_SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("NAME_Camera"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	
	ModeComponent = CreateDefaultSubobject<UMCOPlayerModeComponent>(TEXT("NAME_ModeComponent"));
	
	SetCharacterData();
	SetControlData();

	// Stamina Widget
	WidgetComponent = CreateDefaultSubobject<UMCOWidgetComponent>(TEXT("NAME_WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetRelativeLocation(FVector(20.0f, -50.0f, 160.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> StaminaWidgetRef(TEXT("/Game/UI/WBP_PlayerStamina.WBP_PlayerStamina_C"));
	if (StaminaWidgetRef.Class)
	{
		WidgetComponent->SetWidgetClass(StaminaWidgetRef.Class); 
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); 
		WidgetComponent->SetDrawSize(FVector2D(70.0f, 70.0f)); 
		WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Setting
	PlayerSetting = CreateDefaultSubobject<UMCOPlayerSetting>(TEXT("NAME_PlayerSetting"));

	// Item
	TakeItemActions.Emplace(EMCOItemType::Weapon, FTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &ThisClass::EquipWeapon)
	));
	TakeItemActions.Emplace(EMCOItemType::Potion, FTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &ThisClass::DrinkPotion)
	));
	TakeItemActions.Emplace(EMCOItemType::Scroll, FTakeItemDelegateWrapper(
		FOnTakeItemDelegate::CreateUObject(this, &ThisClass::ReadScroll)
	));

}

void AMCOPlayerCharacter::SetControlData()
{
	ISTRUE(nullptr != CharacterControlData);
	
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;
	
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement; 
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->JumpZVelocity = CharacterControlData->JumpZVelocity;
	GetCharacterMovement()->GravityScale = CharacterControlData->GravityScale;
	
	ISTRUE(true == CharacterControlData->WalkSpeeds.Contains(EMCOCharacterSpeed::Normal));
	GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->WalkSpeeds[EMCOCharacterSpeed::Normal];
	GetCharacterMovement()->MinAnalogWalkSpeed = CharacterControlData->WalkSpeeds[EMCOCharacterSpeed::Normal];
	
	SpringArm->TargetArmLength = CharacterControlData->TargetArmLength;
	SpringArm->SetRelativeLocationAndRotation(CharacterControlData->SpringArmRelativeLocation, CharacterControlData->SpringArmRelativeRotation);
	SpringArm->bInheritPitch = CharacterControlData->bInheritPitch;
	SpringArm->bInheritRoll = CharacterControlData->bInheritRoll;
	SpringArm->bInheritYaw = CharacterControlData->bInheritYaw;
	SpringArm->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
	SpringArm->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
}

void AMCOPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMCOPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->GetOnGameStateChangedDelegate().AddUniqueDynamic(this, &ThisClass::OnGameStateChanged);
	
	ModeComponent->SetCurrentMode(static_cast<int32>(EMCOPlayerMode::TwoHand));
	ModeComponent->SpawnAttachment(this);
}

void AMCOPlayerCharacter::OnGameStateChanged(const EMCOGameState& InState)
{
	if (InState == EMCOGameState::FIGHT)
	{
		bIsMonsterInfoShowed = false;
	}
	else if (InState == EMCOGameState::REWARD)
	{
		RemoveEffectsOnNextGame();
	}
	else if (InState == EMCOGameState::NEXT)
	{
		if (nullptr != GetMCOAbilitySystemComponent())
		{
			GetMCOAbilitySystemComponent()->CancelAbilityByTag(FMCOCharacterTags::Get().TakeItemTag);
		}
	}
	else if (InState == EMCOGameState::RESULT_WIN)
	{
		if (nullptr != GetMCOAbilitySystemComponent() && true == IsAlive())
		{
			if (nullptr != Controller)
			{
				Controller->SetIgnoreMoveInput(true);
			}

			DisableMovement();
			GetMCOAbilitySystemComponent()->CancelAbilities();
			RemoveEffectsOnResult();
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		}
	}
}

void AMCOPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeCharacter();
}

void AMCOPlayerCharacter::InitializeCharacter()
{
	Super::InitializeCharacter();

	bCanTurnByInput = true;
	bCanMoveByInput = true;
	bIsMonsterInfoShowed = false;
	bIsStaminaTimerTicking = false;
	CurrentStaminaForWidget = GetStamina();
	SetSpeed(EMCOCharacterSpeed::Normal);
}

void AMCOPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UMCOInputComponent* MCOInputComponent = CastChecked<UMCOInputComponent>(PlayerInputComponent);
	ensure(MCOInputComponent);
	const APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	ensure(PlayerController);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	MCOInputComponent->SetMappingContext(InputConfig, Subsystem);
		
	MCOInputComponent->BindNativeAction(InputConfig, FMCOCharacterTags::Get().MoveTag, ETriggerEvent::Triggered, this, &ThisClass::Move);
	MCOInputComponent->BindNativeAction(InputConfig, FMCOCharacterTags::Get().MoveTag, ETriggerEvent::Completed, this, &ThisClass::MoveReleased);
	MCOInputComponent->BindNativeAction(InputConfig, FMCOCharacterTags::Get().LookTag, ETriggerEvent::Triggered, this, &ThisClass::Look);

	MCOInputComponent->BindAbilityActions(InputConfig, this, &AMCOPlayerCharacter::Input_AbilityInputTagPressed, &AMCOPlayerCharacter::Input_AbilityInputTagReleased);
}

void AMCOPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	ensure(GetMCOAbilitySystemComponent());
	
	GetMCOAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
}

void AMCOPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	ensure(GetMCOAbilitySystemComponent());
	
	GetMCOAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

FVector AMCOPlayerCharacter::GetInputWorldDirection() const
{
	if (nullptr == Controller)
	{
		return FVector();
	}
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputVector.X;
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputVector.Y;
	
	return (ForwardDirection + RightDirection).GetSafeNormal();
}

bool AMCOPlayerCharacter::IsInputForward() const
{
	return GetInputWorldDirection().Equals(GetActorForwardVector(), 0.05f);
}

EMCOPlayerMode AMCOPlayerCharacter::GetModeType() const
{
	ensure(ModeComponent);
	return static_cast<EMCOPlayerMode>(ModeComponent->GetCurrentMode());
}

bool AMCOPlayerCharacter::IsEquipped()
{
	ISTRUE_F(nullptr != ModeComponent);
	
	return ModeComponent->IsEquipped();
}

void AMCOPlayerCharacter::EquipInstantly()
{
	ISTRUE(nullptr != ModeComponent);
	ModeComponent->SetEquipUnequipInstantly(true);
}

void AMCOPlayerCharacter::SwitchEquipUnequip()
{
	ISTRUE(nullptr != ModeComponent);
	ModeComponent->SwitchEquipUnequip();
}

void AMCOPlayerCharacter::BeginAnimation_Equip()
{
	ISTRUE(nullptr != ModeComponent);

	AMCOAttachment* Attachment = ModeComponent->GetCurrentAttachment();
	ISTRUE(nullptr != Attachment);

	AMCOWeapon* Weapon = Cast<AMCOWeapon>(Attachment);
	ISTRUE(nullptr != Weapon);
	
	Weapon->BeginAnimation_Equip();
}

void AMCOPlayerCharacter::EndAnimation_Equip()
{
	ISTRUE(nullptr != ModeComponent);
	
	AMCOAttachment* Attachment = ModeComponent->GetCurrentAttachment();
	ISTRUE(nullptr != Attachment);

	AMCOWeapon* Weapon = Cast<AMCOWeapon>(Attachment);
	ISTRUE(nullptr != Weapon);
	
	Weapon->EndAnimation_Equip();
}


bool AMCOPlayerCharacter::CheckCanMoveWithTags() const
{
	FGameplayTagContainer TagsThatPlayerCanNotMoveWith;
	// TagsThatPlayerCanNotMoveWith.AddTag(FMCOCharacterTags::Get().EquipTag);
	// TagsThatPlayerCanNotMoveWith.AddTag(FMCOCharacterTags::Get().AttackTag);
	TagsThatPlayerCanNotMoveWith.AddTag(FMCOCharacterTags::Get().DodgeTag);

	return GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TagsThatPlayerCanNotMoveWith) == false;
}

bool AMCOPlayerCharacter::CanMoveCamera() const
{
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(false == HasTag(FMCOCharacterTags::Get().DodgeTag));
	ISTRUE_F(true == bCanTurnByInput);

	return true;
}

bool AMCOPlayerCharacter::CanMoveCharacter() const
{
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == CheckCanMoveWithTags());
	ISTRUE_F(true == bCanMoveByInput);
	ISTRUE_F(false == GetMovementComponent()->IsFalling());

	return true;
}

bool AMCOPlayerCharacter::CanActivateAbility(const FGameplayTag& InTag)
{
	ISTRUE_F(true == Super::CanActivateAbility(InTag));

	if (InTag == FMCOCharacterTags::Get().JumpTag)
	{
		ISTRUE_F(true == IsAlive());
		ISTRUE_F(true == CanJump());
		//ISTRUE_F(true == bGetInput);
		ISTRUE_F(false == GetMovementComponent()->IsFalling());
	}
	else if (InTag == FMCOCharacterTags::Get().EquipTag)
	{
		ISTRUE_F(true == IsAlive());
		//ISTRUE_F(true == bGetInput);
		ISTRUE_F(false == GetMovementComponent()->IsFalling());
	}
	else if (InTag == FMCOCharacterTags::Get().DodgeTag)
	{
		ISTRUE_F(true == IsAlive());
		//ISTRUE_F(true == bGetInput); // removed to cancel attack ability
		ISTRUE_F(false == GetMovementComponent()->IsFalling());
		ISTRUE_F(true == ModeComponent->IsEquipped());
		ISTRUE_F(false == InputVector.IsNearlyZero());
	}
	else if (InTag == FMCOCharacterTags::Get().DashTag)
	{
		ISTRUE_F(true == IsAlive());
		//ISTRUE_F(true == bGetInput);
		ISTRUE_F(false == GetMovementComponent()->IsFalling());
		ISTRUE_F(true == ModeComponent->IsEquipped());
		ISTRUE_F(KINDA_SMALL_NUMBER < InputVector.X);
	}
	else if (InTag == FMCOCharacterTags::Get().AttackTag)
	{
		ISTRUE_F(true == IsAlive());
		//ISTRUE_F(true == bGetInput);
		ISTRUE_F(false == GetMovementComponent()->IsFalling());
		ISTRUE_F(true == ModeComponent->IsEquipped());
	}
	else if (InTag == FMCOCharacterTags::Get().TakeItemTag)
	{
		ISTRUE_F(nullptr != ItemAttributeFragment);
	}
	
	return true;
}

void AMCOPlayerCharacter::SetSpeed(const EMCOCharacterSpeed& InSpeed) const
{
	ISTRUE(nullptr != CharacterControlData);
	ISTRUE(true == CharacterControlData->WalkSpeeds.Contains(InSpeed));
	
	GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->WalkSpeeds[InSpeed];
	GetCharacterMovement()->MinAnalogWalkSpeed = CharacterControlData->WalkSpeeds[InSpeed];
}

void AMCOPlayerCharacter::StopCharacterFromMoving(bool bStopMoving)
{
	bCanMoveByInput = bStopMoving == false;
}

void AMCOPlayerCharacter::StopCharacterFromTurning(bool bStopTuring)
{
	bCanTurnByInput = bStopTuring == false;
}

void AMCOPlayerCharacter::Move(const FInputActionValue& Value)
{
	InputVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ISTRUE(CanMoveCharacter());
	
	if (HasTag(FMCOCharacterTags::Get().DashTag) == true)
	{
		AddMovementInput(ForwardDirection, 1.0f);
		return;
	}
	
	AddMovementInput(ForwardDirection, InputVector.X);
	AddMovementInput(RightDirection, InputVector.Y);
}

void AMCOPlayerCharacter::MoveReleased()
{
	InputVector = FVector2D(0.0f, 0.0f);
}

void AMCOPlayerCharacter::Look(const FInputActionValue& Value)
{
	ISTRUE(CanMoveCamera());
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookAxisVector.X * CharacterControlData->BaseLookRate * GetWorld()->DeltaTimeSeconds);
	AddControllerPitchInput(LookAxisVector.Y * CharacterControlData->BaseLookRate * GetWorld()->DeltaTimeSeconds);
}

// void AMCOPlayerCharacter::SetHUD(UMCOHUDWidget* InHUDWidget)
// {
// 	HUDWidget = InHUDWidget;
// }


void AMCOPlayerCharacter::FinishDying()
{
	Super::FinishDying();
	
	IMCOGameModeInterface* GameModeInterface = Cast<IMCOGameModeInterface>(GetWorld()->GetAuthGameMode());
	ISTRUE(nullptr != GameModeInterface);
	GameModeInterface->OnChangeGameState(EMCOGameState::RESULT_LOSE);	
}

void AMCOPlayerCharacter::TakeItem(const UMCOItemData* InItemData)
{
	ensure(nullptr != InItemData);
	TakeItemActions[InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	
	FGameplayEventData Payload;
	Payload.EventTag       = FMCOCharacterTags::Get().GameplayEvent_TakeItemTag;
	Payload.Instigator     = this;
	Payload.Target         = this;
	GetAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);
}

void AMCOPlayerCharacter::DrinkPotion(const UMCOItemData* InItemData)
{
	const UMCOItemData_Potion* PotionData = Cast<UMCOItemData_Potion>(InItemData); 
	ensure(nullptr != PotionData);
	
	ItemAttributeFragment = PotionData->AttributeFragment;
}

void AMCOPlayerCharacter::EquipWeapon(const UMCOItemData* InItemData)
{
	MCOPRINT(TEXT("Equip Weapon"));
}

void AMCOPlayerCharacter::ReadScroll(const UMCOItemData* InItemData)
{
	MCOPRINT(TEXT("Read Scroll"));
}

UMCOActionFragment_AttributeEffect* AMCOPlayerCharacter::GetItemAttributeFragment()
{
	return ItemAttributeFragment;
}

void AMCOPlayerCharacter::EndTakeItem()
{
	ItemAttributeFragment = nullptr;
}

void AMCOPlayerCharacter::InitializeHUD(UMCOHUDWidget* InHUDWidget)
{
	ISTRUE(nullptr != InHUDWidget);
	HUDWidget = InHUDWidget;
	
	InitializeWidget(InHUDWidget->GetHpWidget(true), InHUDWidget->GetAttributeWidget(true));

	//HUDWidget->ShowInGameWidget(true, true);

	// HUDWidget->InitializeSkillWidget(
	// 	FMCOCharacterTags::Get().JumpTag,
	// 	InputConfig->GetActionKeyName(FMCOCharacterTags::Get().JumpTag)
	// );
	//
	// HUDWidget->InitializeSkillWidget(
	// 	FMCOCharacterTags::Get().DodgeTag,
	// 	InputConfig->GetActionKeyName(FMCOCharacterTags::Get().DodgeTag)
	// );
	//
	// HUDWidget->InitializeSkillWidget(
	// 	FMCOCharacterTags::Get().AttackTag,
	// 	InputConfig->GetActionKeyName(FMCOCharacterTags::Get().AttackTag)
	// );	
}

void AMCOPlayerCharacter::ShowMonsterInfo(IMCOCharacterInterface* InCharacter)
{
	ISTRUE(false == bIsMonsterInfoShowed);

	bIsMonsterInfoShowed = true;
	InCharacter->InitializeWidget(HUDWidget->GetHpWidget(false), HUDWidget->GetAttributeWidget(false));
	HUDWidget->OnMonsterFirstHit();
}

// void AMCOPlayerCharacter::StartCooldownWidget(const FGameplayTag& InTag, const float& InCooldownTime) const
// {
// 	ISTRUE(nullptr != HUDWidget);
// 	HUDWidget->StartSkillWidget(InTag, InCooldownTime);
// }

void AMCOPlayerCharacter::SetupStaminaWidget(UMCOStaminaWidget* InStaminaWidget)
{
	ISTRUE(nullptr != InStaminaWidget);
	
	AMCOPlayerState* MCOPlayerState = Cast<AMCOPlayerState>(GetPlayerState());
	ISTRUE(nullptr != MCOPlayerState);

	MCOPlayerState->BindAttributeChangedDelegate(
		*AttributeSet->GetStaminaAttribute().GetName(), this, &ThisClass::OnStaminaChanged
	);
	
	InStaminaWidget->SetPercent(GetStamina() / GetMaxStamina());	
	OnStaminaChangedDelegate.BindUObject(InStaminaWidget, &UMCOStaminaWidget::SetPercent);

}

void AMCOPlayerCharacter::OnStaminaChanged(float NewStaminaValue)
{
	AdditiveStaminaValueForWidget = NewStaminaValue - CurrentStaminaForWidget;
	ISTRUE(AdditiveStaminaValueForWidget != 0.0f);
	
	//MCOLOG(TEXT("[Stamina] Start : %f -> %f ( %f per sec)"), CurrentStaminaForWidget, NewStaminaValue, AdditiveStaminaValueForWidget);
		
	if (bIsStaminaTimerTicking == false)
	{
		//MCOLOG(TEXT("[Stamina] StartTimer : %f -> %f ( %f per sec)"), CurrentStaminaForWidget, NewStaminaValue, AdditiveStaminaValueForWidget);
		StartStaminaTimer();
	}
}

void AMCOPlayerCharacter::StartStaminaTimer()
{
	bIsStaminaTimerTicking = true;
	
	StaminaTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(
		StaminaTimerHandle,
		this,
		&ThisClass::UpdateStaminaWidget,
		WIDGET_RATE,
		true,
		0.0f
	);
}

void AMCOPlayerCharacter::UpdateStaminaWidget()
{
	CurrentStaminaForWidget += AdditiveStaminaValueForWidget * WIDGET_RATE;
	
	OnStaminaChangedDelegate.ExecuteIfBound(CurrentStaminaForWidget / GetMaxStamina());

	// MCOLOG(TEXT("[Stamina] Updating : %f / %f => %f"), CurrentStaminaForWidget, GetMaxStamina(), GetStamina());
	
	if (FMath::IsNearlyEqual(CurrentStaminaForWidget, GetStamina(), 0.5f))
	{
		//MCOLOG(TEXT("[Stamina] Finish : %f / %f"), CurrentStaminaForWidget, GetMaxStamina());
		StopStaminaTimer();
	}
}

void AMCOPlayerCharacter::StopStaminaTimer()
{
	bIsStaminaTimerTicking = false;
	
	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	
	OnStaminaChangedDelegate.ExecuteIfBound(GetStamina() / GetMaxStamina());

	CurrentStaminaForWidget = GetStamina();
	AdditiveStaminaValueForWidget = 0.0f;
}
