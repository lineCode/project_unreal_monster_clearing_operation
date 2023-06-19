#include "MCOGameplayAbility_CommonAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"
#include "Physics/MCOPhysics.h"
#include "Interface/MCOAttackedInterface.h"
#include "Interface/MCOHUDInterface.h"


UMCOGameplayAbility_CommonAttack::UMCOGameplayAbility_CommonAttack()
{
	GETCLASS(AttributeEffect, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_GiveDamage.GE_GiveDamage_C"));
	
	// Setting 
	bUseOverlapEvent = false;
	bAutoStopCharacter = true;
}

// void UMCOGameplayAbility_CommonAttack::DoneAddingNativeTags()
// {
// 	Super::DoneAddingNativeTags();
// 	
// 	// Blocked by these
// 	ActivationBlockedTags.AddTag(FMCOCharacterTags::Get().DamagedTag);
//
// 	// Cancel these
// 	CancelAbilitiesWithTag.AddTag(FMCOCharacterTags::Get().ChargingTag);
// }

void UMCOGameplayAbility_CommonAttack::StartActivation_CommonAttack(UAnimMontage* InMontage, const FName& InSectionName)
{
	DamagedCharacters.Reset();

	// Play Montage
	StartActivationWithMontage(InMontage, InSectionName);

	// Damage Timer
	SetDamageTimer();
}

void UMCOGameplayAbility_CommonAttack::OnTaskCompleted()
{
	Super::OnTaskCompleted();
	
	ResetDamageTimer();
}

void UMCOGameplayAbility_CommonAttack::OnTaskCancelled()
{
	Super::OnTaskCancelled();
	
	ResetDamageTimer();
}

void UMCOGameplayAbility_CommonAttack::BeginDamaging()
{
	if (CurrentDefinition->AttackTimingFragment->IsMovableWhileGivingDamage(CurrentDamageTimingIdx))
	{
		MCOLOG_C(MCOAbility, TEXT("Move Character !"));
		StopCharacter(false);
	}
	
	StartDamageEndTimer();
	
	if (true == bUseOverlapEvent)
	{
		BeginDamaging_Collision();
	}
	else
	{
		BeginDamaging_Channel();	
	}
}

void UMCOGameplayAbility_CommonAttack::EndDamaging()
{
	if (CurrentDefinition->AttackTimingFragment->IsMovableWhileGivingDamage(CurrentDamageTimingIdx))
	{
		MCOLOG_C(MCOAbility, TEXT("Stop Character !"));
		StopCharacter(true);
	}
	
	CurrentDamageTimingIdx++;
	StartDamageBeginTimer();
	
	if (true == bUseOverlapEvent)
	{
		EndDamaging_Collision();
	}
	else
	{
		EndDamaging_Channel();
	}
}

void UMCOGameplayAbility_CommonAttack::BeginDamaging_Channel()
{
	const float Rate = GetCurrentDamageCheckRate();
	if (Rate <= 0.0f)
	{
		AttackHitCheckByChannel();
		return;
	}
		
	GetWorld()->GetTimerManager().SetTimer(
		DamageByChannelTimerHandle,
		this,
		&ThisClass::AttackHitCheckByChannel,
		Rate,
		true,
		0.0f
	);
}

void UMCOGameplayAbility_CommonAttack::EndDamaging_Channel()
{
}

void UMCOGameplayAbility_CommonAttack::BeginDamaging_Collision()
{
	ensure(nullptr != CurrentDefinition->CollisionFragment);
		
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(nullptr != CharacterInterface);
	
	CharacterInterface->GetAttachmentBeginOverlapDelegate().AddUniqueDynamic(this, &ThisClass::OnAttachmentBeginOverlap);
	CharacterInterface->TurnOnCollision(CurrentDefinition->CollisionFragment->SocketName);
}

void UMCOGameplayAbility_CommonAttack::EndDamaging_Collision()
{
	ensure(nullptr != CurrentDefinition->CollisionFragment);
		
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(nullptr != CharacterInterface);
	
	CharacterInterface->GetAttachmentBeginOverlapDelegate().Clear();
	CharacterInterface->TurnOffCollision(CurrentDefinition->CollisionFragment->SocketName);
}

void UMCOGameplayAbility_CommonAttack::ApplyDamageAndStiffness(ACharacter* InAttackedCharacter)
{
	// Get ASC from AttackedCharacter
	IAbilitySystemInterface* AttackedCharacter = Cast<IAbilitySystemInterface>(InAttackedCharacter);
	ISTRUE(nullptr != AttackedCharacter);
	UAbilitySystemComponent* AttackedASC = AttackedCharacter->GetAbilitySystemComponent();
	ISTRUE(nullptr != AttackedASC);

	// Show Monster Widgets
	IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(CurrentActorInfo->AvatarActor.Get());
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(InAttackedCharacter);
	if (nullptr != HUDInterface && nullptr != CharacterInterface)
	{
		HUDInterface->ShowMonsterInfo(CharacterInterface);
	}

	// Give damage and stiffness 
	FGameplayEffectSpecHandle HandleForAttributes = MakeOutgoingGameplayEffectSpec(AttributeEffect);
	ISTRUE(true == HandleForAttributes.IsValid());
	
	HandleForAttributes.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_DamageTag,
		-CurrentDefinition->AttackTimingFragment->GetDamage(CurrentDamageTimingIdx) // minus!!!!!
	);
	HandleForAttributes.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_StiffnessTag,
		CurrentDefinition->AttackTimingFragment->GetStiffness(CurrentDamageTimingIdx)
	);
	
	AttackedASC->ApplyGameplayEffectSpecToSelf(
		*HandleForAttributes.Data.Get(),
		AttackedASC->GetPredictionKeyForNewAction()
	);

	
	MCOLOG_C(MCOAbility, TEXT("Damage Effect +Health:[%.1f], +Stiffness:[%.1f]"),
		-CurrentDefinition->AttackTimingFragment->GetDamage(CurrentDamageTimingIdx), CurrentDefinition->AttackTimingFragment->GetStiffness(CurrentDamageTimingIdx)
	)
	

	// // For Damaged Ability 
	// FGameplayEffectSpecHandle StiffnessHandle = GetMCOAbilitySystemComponent()->MakeOutgoingSpec(
	// 	TagEffect,
	// 	1,
	// 	EffectContext
	// );
	// ISTRUE(StiffnessHandle.IsValid() == true);
	// // Apply Stiffness Effect to Attacked Character itself
	// // AttackedASC->RemoveActiveGameplayEffect(AttackedASC->RemoveActiveEffectsWithTags(
	// // 	FGameplayTagContainer(FMCOCharacterTags::Get().GameplayEffect_StiffnessTag)
	// // ));
	// AttackedASC->ApplyGameplayEffectSpecToSelf(
	// 	*StiffnessHandle.Data.Get(),
	// 	AttackedASC->GetPredictionKeyForNewAction()
	// );
}

float UMCOGameplayAbility_CommonAttack::CalculateDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& TargetDirection, bool bLog) const
{		
	const FVector ForwardVector = FVector(SourceForward.X, SourceForward.Y, 0.0f);
	const FVector LookVector = FVector(TargetDirection.X, TargetDirection.Y, 0.0f);
	
	const float Dot = FVector::DotProduct(ForwardVector, LookVector);
	const float Acos = FMath::Acos(Dot);
	float Degree = FMath::RadiansToDegrees(Acos);

	const FVector Cross = FVector::CrossProduct(ForwardVector, LookVector);
	if (Cross.Z < 0.0f) // Left is minus
	{
		Degree = -Degree;
	}

	if (true == bLog)
	{
#if ENABLE_DRAW_DEBUG
		DrawDebugLine(
			GetWorld(),
			SourceLocation,
			SourceLocation + ForwardVector * 1000.0f,
			FColor::Red,
			false,
			2.0f
		);
		
		DrawDebugLine(
			GetWorld(),
			SourceLocation,
			SourceLocation + LookVector * 1000.0f,
			FColor::Blue,
			false,
			2.0f
		);
#endif
	}
	
	return Degree;
}

void UMCOGameplayAbility_CommonAttack::SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(InAttackedCharacter);
	ISTRUE(CharacterInterface);
	CharacterInterface->SetDamagedData(CurrentDamagedData);
}

void UMCOGameplayAbility_CommonAttack::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter, const FHitResult& SweepResult)
{
	ISTRUE(nullptr != InAttacker);
	ISTRUE(nullptr != InAttackCauser);
	ISTRUE(nullptr != InAttackedCharacter);
	ISTRUE(false == DamagedCharacters.Contains(InAttackedCharacter));

	const IMCOAttackedInterface* AttackedInterface = Cast<IMCOAttackedInterface>(InAttackedCharacter);
	ISTRUE(nullptr != AttackedInterface);
	
	DamagedCharacters.Emplace(InAttackedCharacter);
			
	CurrentDamagedData.DamagedDegree = CalculateDegree(
		InAttackedCharacter->GetActorLocation(),
		InAttackedCharacter->GetActorForwardVector(),
		-CurrentDefinition->CollisionFragment->GetAttackDirection(InAttacker)
	);

	CurrentDamagedData.DamagedLocation = SweepResult.ImpactPoint;
	CurrentDamagedData.DamagedNiagara = CurrentDefinition->AttackTimingFragment->GetDamageNiagara(CurrentDamageTimingIdx);
	
	SendDamagedDataToTarget(InAttackedCharacter);
	ApplyDamageAndStiffness(InAttackedCharacter);
	
	DamagedCharacters.Reset();
}

void UMCOGameplayAbility_CommonAttack::AttackHitCheckByChannel()
{
	ACharacter* Attacker = GetCharacter();
	ISTRUE(nullptr != Attacker);

	TArray<FHitResult> OutHitResults;
	const FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Attack),
		false,
		Attacker
	);
	
	IMCOCharacterInterface* AttackerInterface = Cast<IMCOCharacterInterface>(Attacker);
	ISTRUE(nullptr != AttackerInterface);

	const FVector Start = AttackerInterface->GetSocketLocation(CurrentDefinition->CollisionFragment->SocketName);
	// const FVector Start = Attacker->GetActorLocation() +
	// 	(Attacker->GetActorForwardVector() * Radius) +
	// 	(Attacker->GetActorForwardVector() * CollisionFragment->AdditiveLocationFromFront);
	
	const FVector AttackDirection = CurrentDefinition->CollisionFragment->GetAttackDirection(Attacker);
	const FVector End = Start + AttackDirection * CurrentDefinition->CollisionFragment->AttackLength;
	
	const FCollisionShape Shape = FCollisionShape::MakeSphere(CurrentDefinition->CollisionFragment->AttackRadius);
	const bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_MCOACTION_TRACE,
		Shape,
		Params
	);
	
#if ENABLE_DRAW_DEBUG
	DrawDebug(AttackDirection, Start, End, HitDetected);
#endif
	
	ISTRUE(true == HitDetected);

	for (const FHitResult & Result : OutHitResults)
	{
		IMCOAttackedInterface* AttackedInterface = Cast<IMCOAttackedInterface>(Result.GetActor());
		if (nullptr == AttackedInterface)
		{
			continue;
		}
		ACharacter* AttackedCharacter = AttackedInterface->GetAttackedCharacter();
		if (nullptr == AttackedCharacter)
		{
			continue;
		}
		if (true == DamagedCharacters.Contains(AttackedCharacter))
		{
			continue;
		}
		if (Attacker == AttackedCharacter)
		{
			continue;
		}
		
		DamagedCharacters.Emplace(AttackedCharacter);
			
		CurrentDamagedData.DamagedDegree = CalculateDegree(
			AttackedCharacter->GetActorLocation(),
			AttackedCharacter->GetActorForwardVector(),
			-AttackDirection
		);

		CurrentDamagedData.DamagedLocation = Result.ImpactPoint;
		CurrentDamagedData.DamagedNiagara = CurrentDefinition->AttackTimingFragment->GetDamageNiagara(CurrentDamageTimingIdx);
		
		SendDamagedDataToTarget(AttackedCharacter);
		ApplyDamageAndStiffness(AttackedCharacter);
	}
	
	DamagedCharacters.Reset();

	// FGameplayAbilityTargetDataHandle TargetDataHandle;
	// FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
	// TargetDataHandle.Add(HitData);
	// TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectSpecToTarget(
	// 	GetCurrentAbilitySpecHandle(),
	// 	CurrentActorInfo,
	// 	GetCurrentActivationInfo(),
	// 	NewHandle,
	// 	TargetDataHandle
	// );
			
	// TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectToTarget(
	// 	GetCurrentAbilitySpecHandle(),
	// 	CurrentActorInfo,
	// 	GetCurrentActivationInfo(),
	// 	TargetDataHandle,
	// 	DamageEffect,
	// 	1,
	// 	1
	// );
}

void UMCOGameplayAbility_CommonAttack::DrawDebug(const FVector& AttackForward, const FVector& Start, const FVector& End, bool bHitDetected) const
{
	const FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	const float CapsuleHalfHeight = CurrentDefinition->CollisionFragment->AttackLength * 0.5f;
	const FColor DrawColor = bHitDetected == true ? FColor::Green : FColor::Red;

	DrawDebugCapsule(
		GetWorld(), 
		CapsuleOrigin,
		CapsuleHalfHeight,
		CurrentDefinition->CollisionFragment->AttackRadius,
		FRotationMatrix::MakeFromZ(AttackForward).ToQuat(), 
		DrawColor,
		false, 
		0.5f 
	);
}

void UMCOGameplayAbility_CommonAttack::SetDamageTimer()
{
	CurrentDamageTimingIdx = 0;
	StartDamageBeginTimer();
}

void UMCOGameplayAbility_CommonAttack::ResetDamageTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DamageByChannelTimerHandle);
}

float UMCOGameplayAbility_CommonAttack::GetCurrentDamageBeginFrameCount() const
{
	return CurrentDefinition->AttackTimingFragment->GetDamageBeginTimeAfterPrevEndTime(CurrentDamageTimingIdx);
}

float UMCOGameplayAbility_CommonAttack::GetCurrentDamageEndFrameCount() const
{
	return CurrentDefinition->AttackTimingFragment->GetDamageExistTime(CurrentDamageTimingIdx);
}

float UMCOGameplayAbility_CommonAttack::GetCurrentDamageCheckRate() const
{
	return CurrentDefinition->AttackTimingFragment->GetDamageCheckRate(CurrentDamageTimingIdx);
}

void UMCOGameplayAbility_CommonAttack::StartDamageBeginTimer()
{
	ResetDamageTimer();
	
	const float FrameCount = GetCurrentDamageBeginFrameCount();

	MCOLOG_C(MCOAbility, TEXT("... To Start : %f sec later "), FrameCount);
	
	ISTRUE(FrameCount > 0.0f);
		
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ThisClass::BeginDamaging,
		FrameCount,
		false
	);
}

void UMCOGameplayAbility_CommonAttack::StartDamageEndTimer()
{
	ResetDamageTimer();

	const float FrameCount = GetCurrentDamageEndFrameCount();

	MCOLOG_C(MCOAbility, TEXT("... To End : %f sec later "), FrameCount);
	
	ISTRUE(FrameCount > 0.0f);
	
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ThisClass::EndDamaging,
		FrameCount,
		false
	);
}
//virtual FActiveGameplayEffectHandle UAbilitySystemComponent::ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey = FPredictionKey());
//virtual FActiveGameplayEffectHandle UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect,	FPredictionKey PredictionKey = FPredictionKey());
