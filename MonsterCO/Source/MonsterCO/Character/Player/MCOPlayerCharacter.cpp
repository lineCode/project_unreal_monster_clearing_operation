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
#include "UI/MCOHUDWidget.h"
#include "MCOPlayerControlData.h"
#include "Character/MCOCharacterData.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/MCOAbilitySystemComponent.h"
#include "AbilitySystem/MCOAttributeSet.h"
#include "Character/MCOPlayerState.h"
#include "CharacterAttachment/MCOPlayerModeComponent.h"
#include "CharacterAttachment/MCOWeapon.h"
#include "UI/MCOStaminaWidget.h"
#include "UI/MCOWidgetComponent.h"


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

	ModeComponent->SpawnWeapon(this);
	ModeComponent->SetMode(EMCOModeType::TwoHand);
	InitializeHUD();
}

void AMCOPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	Initialize();
}

void AMCOPlayerCharacter::Initialize()
{
	Super::Initialize();
	
	bGetInput = true;
	bIsMonsterInfoShowed = false;
	bIsStaminaTimerTicking = false;
	SetSpeed(EMCOCharacterSpeed::Normal);
	CurrentStaminaForWidget = GetStamina();
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
		
	MCOInputComponent->BindNativeAction(InputConfig, FMCOCharacterTags::Get().MoveTag, ETriggerEvent::Triggered, this, &AMCOPlayerCharacter::Move);
	MCOInputComponent->BindNativeAction(InputConfig, FMCOCharacterTags::Get().LookTag, ETriggerEvent::Triggered, this, &AMCOPlayerCharacter::Look);

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

FVector AMCOPlayerCharacter::GetInputDirection() const
{
	const FVector ForwardDirection = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * MovementVector.X;
	const FVector RightDirection = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y) * MovementVector.Y;

	return ForwardDirection + RightDirection;
}

bool AMCOPlayerCharacter::IsDashForward() const
{
	return (MovementVector.X > KINDA_SMALL_NUMBER) && (FMath::IsNearlyZero(MovementVector.Y) == true);
}

EMCOModeType AMCOPlayerCharacter::GetModeType() const
{
	return ModeComponent->GetModeType();
}

void AMCOPlayerCharacter::OffAllCollision()
{
	Super::OffAllCollision();

	
}

bool AMCOPlayerCharacter::IsEquipped()
{
	ISTRUE_F(nullptr != ModeComponent);
	
	return ModeComponent->IsEquipped();
}

void AMCOPlayerCharacter::SetEquippedWithoutAnimation()
{
	ISTRUE(nullptr != ModeComponent);
	ModeComponent->SetEquip();
}

void AMCOPlayerCharacter::SwitchEquipUnequip()
{
	ISTRUE(nullptr != ModeComponent);
	ModeComponent->SwitchEquipUnequip();
}

void AMCOPlayerCharacter::BeginAnimation_Equip()
{
	ISTRUE(nullptr != ModeComponent);
	ISTRUE(nullptr != ModeComponent->GetWeapon());
	ModeComponent->GetWeapon()->BeginAnimation_Equip();
}

void AMCOPlayerCharacter::EndAnimation_Equip()
{
	ISTRUE(nullptr != ModeComponent);
	ISTRUE(nullptr != ModeComponent->GetWeapon());
	ModeComponent->GetWeapon()->EndAnimation_Equip();
}


bool AMCOPlayerCharacter::CheckCanMoveWithTags() const
{
	FGameplayTagContainer TagsThatPlayerCanNotMoveWith;
	TagsThatPlayerCanNotMoveWith.AddTag(FMCOCharacterTags::Get().EquipTag);
	TagsThatPlayerCanNotMoveWith.AddTag(FMCOCharacterTags::Get().AttackTag);
	TagsThatPlayerCanNotMoveWith.AddTag(FMCOCharacterTags::Get().DodgeTag);
	//TagsThatPlayerCanNotMoveWith.AddTag(FMCOPlayerTags::Get().DashTag);

	return GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TagsThatPlayerCanNotMoveWith) == false;
}

bool AMCOPlayerCharacter::CanMoveCamera() const
{
	ISTRUE_F(IsAlive());
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().DodgeTag) == false);

	return true;
}

bool AMCOPlayerCharacter::CanMoveCharacter() const
{
	ISTRUE_F(true == CheckCanMoveWithTags());
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == bGetInput);
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);

	return true;
}

bool AMCOPlayerCharacter::CanJumpAction() const
{
	ISTRUE_F(true == CanJump());
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == bGetInput);
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);

	return true;
}

bool AMCOPlayerCharacter::CanEquipAction() const
{
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == bGetInput);
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);

	return true;
}

bool AMCOPlayerCharacter::CanDodgeAction() const
{
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == bGetInput);
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);
	ISTRUE_F(ModeComponent->IsEquipped() == true);
	ISTRUE_F((FMath::IsNearlyZero(MovementVector.X) == false || FMath::IsNearlyZero(MovementVector.Y) == false));

	return true;
}

bool AMCOPlayerCharacter::CanDashAction() const
{
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == bGetInput);
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);
	ISTRUE_F(ModeComponent->IsEquipped() == true);
	ISTRUE_F(UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::W));

	return true;
}

bool AMCOPlayerCharacter::CanAttack() const
{
	Super::CanAttack();
	
	ISTRUE_F(true == IsAlive());
	ISTRUE_F(true == bGetInput);
	ISTRUE_F(HasTag(FMCOCharacterTags::Get().StunTag) == false);
	ISTRUE_F(GetMovementComponent()->IsFalling() == false);
	ISTRUE_F(ModeComponent->IsEquipped() == true);

	return true;
}

// void AMCOPlayerCharacter::CancelDash() const
// {
// 	ISTRUE(nullptr != AbilitySystemComponent);
//
// 	FGameplayTagContainer EffectsTagToRemove;
// 	EffectsTagToRemove.AddTag(FMCOCharacterTags::Get().DashTag);
// 	AbilitySystemComponent->CancelAbilities(&EffectsTagToRemove);
// }

void AMCOPlayerCharacter::SetSpeed(const EMCOCharacterSpeed& InSpeed) const
{
	ISTRUE(nullptr != CharacterControlData);
	ISTRUE(true == CharacterControlData->WalkSpeeds.Contains(InSpeed));
	
	GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->WalkSpeeds[InSpeed];
	GetCharacterMovement()->MinAnalogWalkSpeed = CharacterControlData->WalkSpeeds[InSpeed];
}

void AMCOPlayerCharacter::StopCharacter(bool bToStop)
{
	bGetInput = bToStop == false;
}

void AMCOPlayerCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ISTRUE(CanMoveCharacter());
	
	if (HasTag(FMCOCharacterTags::Get().DashTag) == true)
	{
		AddMovementInput(ForwardDirection, 1.0f);
		return;
		
		//if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::W))
		//{
		//	AddMovementInput(ForwardDirection, 1.0f);
		//	return;
		//}
		//CancelDash();
	}
	
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMCOPlayerCharacter::Look(const FInputActionValue& Value)
{
	ISTRUE(CanMoveCamera());
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookAxisVector.X * CharacterControlData->BaseLookRate * GetWorld()->DeltaTimeSeconds);
	AddControllerPitchInput(LookAxisVector.Y * CharacterControlData->BaseLookRate * GetWorld()->DeltaTimeSeconds);
}

void AMCOPlayerCharacter::SetHUD(UMCOHUDWidget* InHUDWidget)
{
	HUDWidget = InHUDWidget;
}

void AMCOPlayerCharacter::InitializeHUD()
{
	ISTRUE(nullptr != HUDWidget);

	HUDWidget->InitializeHUDWidget(true);
	InitializeWidget(HUDWidget->GetHpWidget(true), HUDWidget->GetAttributeWidget(true));
	HUDWidget->ShowWidget(true, true);

	// temp
	HUDWidget->InitializeSkillWidget(
		FMCOCharacterTags::Get().DodgeTag,
		InputConfig->GetActionKeyName(FMCOCharacterTags::Get().DodgeTag)
	);
	
	HUDWidget->InitializeSkillWidget(
		FMCOCharacterTags::Get().AttackTag,
		InputConfig->GetActionKeyName(FMCOCharacterTags::Get().AttackTag)
	);
}

void AMCOPlayerCharacter::ShowMonsterInfo(IMCOCharacterInterface* InCharacter)
{
	ISTRUE(false == bIsMonsterInfoShowed);
	ISTRUE(nullptr != HUDWidget);
	
	bIsMonsterInfoShowed = true;
	HUDWidget->InitializeHUDWidget(false);
	InCharacter->InitializeWidget(HUDWidget->GetHpWidget(false), HUDWidget->GetAttributeWidget(false));
	HUDWidget->ShowWidget(false, true);
}

void AMCOPlayerCharacter::StartCooldownWidget(const FGameplayTag& InTag, const float& InCooldownTime) const
{
	ISTRUE(nullptr != HUDWidget);
	HUDWidget->StartSkillWidget(InTag, InCooldownTime);
}

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
	
	MCOLOG(TEXT("[Stamina] Start : %f -> %f ( %f per sec)"), CurrentStaminaForWidget, NewStaminaValue, AdditiveStaminaValueForWidget);
		
	if (bIsStaminaTimerTicking == false)
	{
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
		MCOLOG(TEXT("[Stamina] Finish : %f / %f"), CurrentStaminaForWidget, GetMaxStamina());
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
