#include "MCOGA_CommonAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"
#include "Physics/MCOPhysics.h"
#include "Interface/MCOAttackedInterface.h"
#include "Interface/MCOHUDInterface.h"
#include "Projectile/MCOProjectile.h"


UMCOGA_CommonAttack::UMCOGA_CommonAttack()
{
	GETCLASS(InstantEffectWithCue, UGameplayEffect, TEXT("/Game/AbilitySystem/Effects/GE_GiveDamage.GE_GiveDamage_C"));
	GETCLASS(DurationEffectWithCue, UGameplayEffect, TEXT("/Game/AbilitySystem/Effects/GE_GiveDurationDamage.GE_GiveDurationDamage_C"));
	
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

void UMCOGA_CommonAttack::StartActivation_CommonAttack(UAnimMontage* InMontage, const FName& InSectionName)
{
	DamagedCharacters.Reset();

	// Play Montage
	StartActivationWithMontage(InMontage, InSectionName);

	// Damage Timer
	SetDamageTimer();
}

void UMCOGA_CommonAttack::OnTaskCompleted()
{
	Super::OnTaskCompleted();
	
	ResetDamageTimer();
}

void UMCOGA_CommonAttack::OnTaskCancelled()
{
	Super::OnTaskCancelled();
	
	ResetDamageTimer();
}

void UMCOGA_CommonAttack::BeginDamaging()
{
	ResetDamageTimer();
	SetMovementOnBeginDamaging();

	// Set End Timer
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

void UMCOGA_CommonAttack::EndDamaging()
{
	ResetDamageTimer();
	SetMovementOnEndDamaging();

	// Next Damage Timing : Set Begin Timer
	CurrentDamageTimingIdx++;
	MCOLOG_C(MCOAbility, TEXT("[%d] ... Idx++ "), CurrentDamageTimingIdx);
	if (true == CurrentDefinition->AttackTimingFragment->IsValidIdx(CurrentDamageTimingIdx))
	{
		StartDamageBeginTimer();
	}
	
	if (true == bUseOverlapEvent)
	{
		EndDamaging_Collision();
	}
	else
	{
		EndDamaging_Channel();
	}
}

void UMCOGA_CommonAttack::BeginDamaging_Channel()
{
	const float Rate = GetCurrentDamageCheckRate();
	if (Rate <= 0.0f)
	{
		Attack();
		return;
	}
		
	GetWorld()->GetTimerManager().SetTimer(
		DamageByChannelTimerHandle,
		this,
		&ThisClass::Attack,
		Rate,
		true,
		0.0f
	);
}

void UMCOGA_CommonAttack::EndDamaging_Channel()
{
}

void UMCOGA_CommonAttack::BeginDamaging_Collision()
{
	ensure(nullptr != CurrentDefinition->CollisionFragment);
		
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(nullptr != CharacterInterface);
	
	CharacterInterface->GetCollisionBeginOverlapDelegate().AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlap);
	CharacterInterface->TurnOnCollision(CurrentDefinition->CollisionFragment->SocketName);
}

void UMCOGA_CommonAttack::EndDamaging_Collision()
{
	ensure(nullptr != CurrentDefinition->CollisionFragment);
		
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(nullptr != CharacterInterface);
	
	CharacterInterface->GetCollisionBeginOverlapDelegate().Clear();
	CharacterInterface->TurnOffCollision(CurrentDefinition->CollisionFragment->SocketName);
}

void UMCOGA_CommonAttack::ApplyDamageAndStiffness(ACharacter* InAttackedCharacter)
{
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->AttackTimingFragment);
	
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

	const UMCOActionFragment_AttributeEffect* AttributeFragment = CurrentDefinition->AttackTimingFragment->GetAttributeFragment(CurrentDamageTimingIdx);
	
	ApplyEffect(AttackedASC, AttributeFragment, EMCOEffectPolicy::Instant, InstantEffectWithCue);
	ApplyEffect(AttackedASC, AttributeFragment, EMCOEffectPolicy::Duration, DurationEffectWithCue);
}

float UMCOGA_CommonAttack::CalculateDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& TargetDirection, bool bLog) const
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

void UMCOGA_CommonAttack::SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(InAttackedCharacter);
	ISTRUE(CharacterInterface);
	CharacterInterface->SetDamagedData(CurrentDamagedData);
}

void UMCOGA_CommonAttack::Attack()
{
	MCOLOG_C(MCOAbility, TEXT("[%d] ... Attack !"), CurrentDamageTimingIdx);
	
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->AttackTimingFragment);
	
	if (CurrentDefinition->AttackTimingFragment->IsUsingProjectile(CurrentDamageTimingIdx))
	{
		AttackByProjectile();
	}
	else
	{
		AttackByInstantCheck();
	}
}

void UMCOGA_CommonAttack::OnCollisionBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter, const FHitResult& SweepResult)
{
	ISTRUE(nullptr != InAttacker);
	ISTRUE(nullptr != InAttackCauser);
	ISTRUE(nullptr != InAttackedCharacter);
	ISTRUE(false == DamagedCharacters.Contains(InAttackedCharacter));
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->CollisionFragment);
	ISTRUE(nullptr != CurrentDefinition->AttackTimingFragment);

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
	CurrentDamagedData.DurationNiagara = CurrentDefinition->AttackTimingFragment->GetDurationEffectNiagara(CurrentDamageTimingIdx);
	
	SendDamagedDataToTarget(InAttackedCharacter);
	ApplyDamageAndStiffness(InAttackedCharacter);
	
	DamagedCharacters.Reset();
}

void UMCOGA_CommonAttack::AttackByProjectile()
{
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->CollisionFragment);
	ISTRUE(nullptr != CurrentDefinition->AttackTimingFragment);
	
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE(CharacterInterface);

	FTransform SocketTransform = CharacterInterface->GetSocketTransform(CurrentDefinition->CollisionFragment->SocketName);
	SocketTransform.SetRotation(FQuat(FRotator(
		0.0f,
		SocketTransform.GetRotation().Rotator().Yaw,
		SocketTransform.GetRotation().Rotator().Roll
	)));

	AMCOProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AMCOProjectile>(
		*CurrentDefinition->AttackTimingFragment->GetProjectileClass(CurrentDamageTimingIdx),
		SocketTransform,
		CurrentActorInfo->AvatarActor.Get(),
		Cast<APawn>(CurrentActorInfo->AvatarActor.Get()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	// bind overlap event 
	SpawnedProjectile->CollisionBeginOverlapDelegate.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlap);

	// initialize 
	SpawnedProjectile->Initialize(
		CurrentDefinition->AttackTimingFragment->GetProjectileSpeed(CurrentDamageTimingIdx),
		CurrentDefinition->AttackTimingFragment->GetProjectileLifeSpan(CurrentDamageTimingIdx)
	);
	
	SpawnedProjectile->FinishSpawning(SocketTransform);
}

void UMCOGA_CommonAttack::AttackByInstantCheck()
{
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->CollisionFragment);
	ISTRUE(nullptr != CurrentDefinition->AttackTimingFragment);
	
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
		CHANNEL_ACTION_TRACE,
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
		CurrentDamagedData.DurationNiagara = CurrentDefinition->AttackTimingFragment->GetDurationEffectNiagara(CurrentDamageTimingIdx);
		
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

void UMCOGA_CommonAttack::DrawDebug(const FVector& AttackForward, const FVector& Start, const FVector& End, bool bHitDetected) const
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

void UMCOGA_CommonAttack::SetMovementOnBeginDamaging() const
{
	ensure(nullptr != CurrentDefinition);
	ensure(nullptr != CurrentDefinition->AttackTimingFragment);

	if (!bAutoStopCharacter != CurrentDefinition->AttackTimingFragment->IsMovable(CurrentDamageTimingIdx))
	{
		MCOLOG_C(MCOAbility, TEXT("Can Move"));
		StopCharacterFromMoving(false);
	}

	if (false == CurrentDefinition->AttackTimingFragment->CanTurn(CurrentDamageTimingIdx))
	{
		MCOLOG_C(MCOAbility, TEXT("Stop Turn"));
		StopCharacterFromTurning(true);
	}
}

void UMCOGA_CommonAttack::SetMovementOnEndDamaging() const
{
	ensure(nullptr != CurrentDefinition);
	ensure(nullptr != CurrentDefinition->AttackTimingFragment);

	if (!bAutoStopCharacter != CurrentDefinition->AttackTimingFragment->IsMovable(CurrentDamageTimingIdx))
	{
		MCOLOG_C(MCOAbility, TEXT("Stop Move"));
		StopCharacterFromMoving(true);
	}
	
	if (false == CurrentDefinition->AttackTimingFragment->CanTurn(CurrentDamageTimingIdx))
	{
		MCOLOG_C(MCOAbility, TEXT("Can Turn"));
		StopCharacterFromTurning(false);
	}
}

void UMCOGA_CommonAttack::SetDamageTimer()
{
	CurrentDamageTimingIdx = 0;
	ResetDamageTimer();
	StartDamageBeginTimer();
}

void UMCOGA_CommonAttack::ResetDamageTimer()
{
	MCOLOG_C(MCOAbility, TEXT("... Reset Timer "));
	
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DamageByChannelTimerHandle);
}

float UMCOGA_CommonAttack::GetCurrentDamageBeginFrameCount() const
{
	return CurrentDefinition->AttackTimingFragment->GetDamageBeginTimeAfterPrevEndTime(CurrentDamageTimingIdx);
}

float UMCOGA_CommonAttack::GetCurrentDamageEndFrameCount() const
{
	return CurrentDefinition->AttackTimingFragment->GetDamageExistTime(CurrentDamageTimingIdx);
}

float UMCOGA_CommonAttack::GetCurrentDamageCheckRate() const
{
	return CurrentDefinition->AttackTimingFragment->GetDamageCheckRate(CurrentDamageTimingIdx);
}

void UMCOGA_CommonAttack::StartDamageBeginTimer()
{
	const float FrameCount = GetCurrentDamageBeginFrameCount();

	MCOLOG_C(MCOAbility, TEXT("[%d] ... To Start : %f sec later "), CurrentDamageTimingIdx, FrameCount);
	
	ISTRUE(FrameCount > 0.0f);
		
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ThisClass::BeginDamaging,
		FrameCount,
		false
	);
}

void UMCOGA_CommonAttack::StartDamageEndTimer()
{
	const float FrameCount = GetCurrentDamageEndFrameCount();

	MCOLOG_C(MCOAbility, TEXT("[%d] ... To End : %f sec later "), CurrentDamageTimingIdx, FrameCount);
	
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
