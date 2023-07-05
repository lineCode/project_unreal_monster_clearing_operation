#include "MCOGA_CommonAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttackTiming.h"
#include "AbilitySystem/ActionData/MCOActionFragment_AttributeEffect.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"
#include "Physics/MCOPhysics.h"
#include "Interface/MCOAttackedInterface.h"
#include "Interface/MCOHUDInterface.h"
#include "Projectile/MCOProjectile.h"


UMCOGA_CommonAttack::UMCOGA_CommonAttack()
{
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

void UMCOGA_CommonAttack::StartAttackActivation(UAnimMontage* InMontage, const FName& InSectionName)
{
	DamagedCharacters.Reset();
	
	if (true == bHasJumpRootMotion)
	{
		IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
		if (nullptr != CharacterInterface)
		{
			CharacterInterface->ChangedMovementMode(MOVE_Flying);
		}
	}
	
	if (nullptr != InMontage)
	{
		// Play Montage
		StartActivationWithMontage(InMontage, InSectionName);
	}

	// Damage Timer
	SetDamageTimer();
}

void UMCOGA_CommonAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bHasJumpRootMotion)
	{
		IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
		if (nullptr != CharacterInterface)
		{
			CharacterInterface->ChangedMovementMode(MOVE_Walking);
		}
	}
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

	if (true == CurrentDefinition->AttackTimingFragment->IsValidIdx(CurrentDamageTimingIdx + 1))
	{
		// Next Damage Timing : Set Begin Timer
		CurrentDamageTimingIdx++;
		StartDamageBeginTimer();
		
		//MCOLOG_C(MCOTimer, TEXT("[%d] ... Idx++ "), CurrentDamageTimingIdx);
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
	const UMCOActionFragment_Collision* CollisionFragment = GetCollisionFragment(CurrentDamageTimingIdx);
	ensure(CollisionFragment);
		
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(nullptr != CharacterInterface);

	FCollisionBeginOverlapDelegate OnBeginDelegate;
	OnBeginDelegate.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlap);
	
	FCollisionEndOverlapDelegate OnEndDelegate;
	
	CharacterInterface->OnBeginCollision(OnBeginDelegate, OnEndDelegate, CollisionFragment->SocketName);
}

void UMCOGA_CommonAttack::EndDamaging_Collision()
{
	const UMCOActionFragment_Collision* CollisionFragment = GetCollisionFragment(CurrentDamageTimingIdx);
	ensure(nullptr != CollisionFragment);
	
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ensure(nullptr != CharacterInterface);
	
	CharacterInterface->OnEndCollision(CollisionFragment->SocketName);
}

void UMCOGA_CommonAttack::ApplyDamageAndStiffness(ACharacter* InAttackedCharacter, float InDamagedDegree, const FHitResult& InHitResult)
{
	// Get ASC from AttackedCharacter
	IAbilitySystemInterface* AttackedCharacter = Cast<IAbilitySystemInterface>(InAttackedCharacter);
	ISTRUE(nullptr != AttackedCharacter);
	UAbilitySystemComponent* AttackedASC = AttackedCharacter->GetAbilitySystemComponent();
	ISTRUE(nullptr != AttackedASC);

	// Show Monster Widgets
	IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(GetActor());
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(InAttackedCharacter);
	if (nullptr != HUDInterface && nullptr != CharacterInterface)
	{
		HUDInterface->ShowMonsterInfo(CharacterInterface);
	}

	const UMCOActionFragment_AttributeEffect* AttributeFragment = GetAttackAttributeFragment(CurrentDamageTimingIdx);
	ISTRUE(nullptr != AttributeFragment);

	for (const EMCOEffectPolicy& Policy : TEnumRange<EMCOEffectPolicy>())
	{
		SendDamagedDataToTarget(InAttackedCharacter, InDamagedDegree);

		if (false == EffectClassesForAttack.Contains(Policy))
		{
			continue;
		}
		
		ApplyEffectWithHitResult(
			AttackedASC,
			AttributeFragment,
			Policy,
			EffectClassesForAttack[Policy],
			InHitResult
		);
	}
}

bool UMCOGA_CommonAttack::ApplyEffectWithHitResult(	
	UAbilitySystemComponent* ASC,
	const UMCOActionFragment_AttributeEffect* AttributeFragment,
	const EMCOEffectPolicy& InPolicy,
	const TSubclassOf<UGameplayEffect>& EffectClass,
	const FHitResult& InHitResult) const
{
	ISTRUE_F(nullptr != ASC);
	ISTRUE_F(nullptr != EffectClass);
	ISTRUE_F(nullptr != AttributeFragment);
	ISTRUE_F(true == AttributeFragment->IsEffectExistByPolicy(InPolicy));
	
	const FGameplayEffectSpecHandle NewHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
	ISTRUE_F(true == NewHandle.IsValid());

	AttributeFragment->ApplyAttributeAdditiveValue(InPolicy, NewHandle);

	// Set hit result for cue location 
	if (false == InHitResult.ImpactPoint.IsNearlyZero())
	{
		NewHandle.Data->GetContext().AddHitResult(InHitResult);
	}
	
	ASC->ApplyGameplayEffectSpecToSelf(
		*NewHandle.Data.Get(),
		ASC->GetPredictionKeyForNewAction()
	);

	return true;
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

void UMCOGA_CommonAttack::SendDamagedDataToTarget(ACharacter* InAttackedCharacter, float InDegree) const
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(InAttackedCharacter);
	ISTRUE(CharacterInterface);
	CharacterInterface->SetDamagedDegree(InDegree);
}

void UMCOGA_CommonAttack::Attack()
{
	//MCOLOG_C(MCOTimer, TEXT("[%d] ... Attack !"), CurrentDamageTimingIdx);
	
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

	if (true == bIsInstantAttack)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMCOGA_CommonAttack::OnCollisionBeginOverlap(ACharacter* InAttacker, ACharacter* InAttackedCharacter, const FHitResult& SweepResult)
{
	ISTRUE(nullptr != InAttacker);
	ISTRUE(nullptr != InAttackedCharacter);
	ISTRUE(false == DamagedCharacters.Contains(InAttackedCharacter));

	const UMCOActionFragment_Collision* CollisionFragment = GetCollisionFragment(CurrentDamageTimingIdx);
	ISTRUE(nullptr != CollisionFragment);
	
	const IMCOAttackedInterface* AttackedInterface = Cast<IMCOAttackedInterface>(InAttackedCharacter);
	ISTRUE(nullptr != AttackedInterface);
	
	DamagedCharacters.Emplace(InAttackedCharacter);
	
	const float DamagedDegree = CalculateDegree(
		InAttackedCharacter->GetActorLocation(),
		InAttackedCharacter->GetActorForwardVector(),
		-CollisionFragment->GetAttackDirection(InAttacker)
	);
		
	ApplyDamageAndStiffness(InAttackedCharacter, DamagedDegree, SweepResult);
	
	DamagedCharacters.Reset();
}

void UMCOGA_CommonAttack::AttackByProjectile()
{
	ISTRUE(nullptr != CurrentDefinition);
	ISTRUE(nullptr != CurrentDefinition->AttackTimingFragment);
	
	const UMCOActionFragment_Collision* CollisionFragment = GetCollisionFragment(CurrentDamageTimingIdx);
	ISTRUE(nullptr != CollisionFragment);
	
	IMCOCharacterInterface* CharacterInterface = GetMCOCharacterInterface();
	ISTRUE(CharacterInterface);

	AMCOProjectile* SpawnedProjectile = CharacterInterface->SpawnProjectile(
		*CurrentDefinition->AttackTimingFragment->GetProjectileClass(CurrentDamageTimingIdx),
		CollisionFragment->SocketName,
		CurrentDefinition->AttackTimingFragment->GetProjectileSpeed(CurrentDamageTimingIdx),
		CurrentDefinition->AttackTimingFragment->GetProjectileLifeSpan(CurrentDamageTimingIdx)
	);

	ISTRUE(nullptr != SpawnedProjectile);
	
	// bind overlap event 
	SpawnedProjectile->CollisionBeginOverlapDelegate.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlap);

}

void UMCOGA_CommonAttack::AttackByInstantCheck()
{
	const UMCOActionFragment_Collision* CollisionFragment = GetCollisionFragment(CurrentDamageTimingIdx);
	ISTRUE(nullptr != CollisionFragment);
	
	AActor* Attacker = GetActor();
	ISTRUE(nullptr != Attacker);
	IMCOCharacterInterface* AttackerInterface = Cast<IMCOCharacterInterface>(Attacker);
	ISTRUE(nullptr != AttackerInterface);

	
	TArray<AActor*> IgnoreActors;
	AttackerInterface->GetIgnoreActors(IgnoreActors);
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Attack),
		false
	);
	Params.AddIgnoredActors(IgnoreActors);
	
	
	const FName SocketName = CollisionFragment->SocketName;
	const FVector Start = (SocketName != NAME_None) ? AttackerInterface->GetSocketLocation(SocketName) : Attacker->GetActorLocation();
	const FVector AttackDirection = CollisionFragment->GetAttackDirection(Attacker);
	const FVector End = Start + AttackDirection * CollisionFragment->AttackLength;
	
	TArray<FHitResult> OutHitResults;
	
	const FCollisionShape Shape = FCollisionShape::MakeSphere(CollisionFragment->AttackRadius);
	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_ACTION_TRACE,
		Shape,
		Params
	);
	
#if ENABLE_DRAW_DEBUG

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		Start,
		End,
		CollisionFragment->AttackRadius,
		UEngineTypes::ConvertToTraceType(CHANNEL_ACTION_TRACE),
		true,
		IgnoreActors,
		EDrawDebugTrace::Type::ForDuration,
		OutHitResults,
		true, 
		FColor::Red,
		FColor::Green,
		1.0f
	);
	
#endif
	
	ISTRUE(true == HitDetected);
	
	for (const FHitResult& Result : OutHitResults)
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
		if (Attacker == AttackedCharacter)
		{
			continue;
		}
		if (true == DamagedCharacters.Contains(AttackedCharacter))
		{
			continue;
		}
		IMCOCharacterInterface* AttackedCharacterInterface = Cast<IMCOCharacterInterface>(AttackedCharacter);
		if (nullptr == AttackedCharacterInterface)
		{
			continue;
		}
		if (false == AttackedCharacterInterface->CheckCanBeDamaged(AbilityTag))
		{
			continue;
		}
		
		DamagedCharacters.Emplace(AttackedCharacter);
			
		const float DamagedDegree = CalculateDegree(
			AttackedCharacter->GetActorLocation(),
			AttackedCharacter->GetActorForwardVector(),
			-AttackDirection
		);
		
		ApplyDamageAndStiffness(AttackedCharacter, DamagedDegree, Result);
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
	const UMCOActionFragment_Collision* CollisionFragment = GetCollisionFragment(CurrentDamageTimingIdx);
	ensure(nullptr != CollisionFragment);
		
	const FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	const float CapsuleHalfHeight = CollisionFragment->AttackLength * 0.5f;
	const FColor DrawColor = bHitDetected == true ? FColor::Green : FColor::Red;

	DrawDebugCapsule(
		GetWorld(), 
		CapsuleOrigin,
		CapsuleHalfHeight,
		CollisionFragment->AttackRadius,
		FRotationMatrix::MakeFromZ(AttackForward).ToQuat(), 
		DrawColor,
		false, 
		0.5f 
	);
}

UMCOActionFragment_AttributeEffect* UMCOGA_CommonAttack::GetAttackAttributeFragment(const uint8& InDamageIdx) const
{
	ISTRUE_N(nullptr != CurrentDefinition);
	ISTRUE_N(nullptr != CurrentDefinition->AttackTimingFragment);

	return CurrentDefinition->AttackTimingFragment->GetAttributeFragment(InDamageIdx);
}

UMCOActionFragment_Collision* UMCOGA_CommonAttack::GetCollisionFragment(const uint8& InDamageIdx) const
{
	ISTRUE_N(nullptr != CurrentDefinition);

	// for using different collision data
	if (nullptr != CurrentDefinition->AttackTimingFragment)
	{
		UMCOActionFragment_Collision* CollisionFragment = CurrentDefinition->AttackTimingFragment->GetCollisionFragment(InDamageIdx);
		if (nullptr != CollisionFragment)
		{
			return CollisionFragment;
		}
	}

	// for using same collision data 
	return CurrentDefinition->CollisionFragment;
}

UMCOActionFragment_Projectile* UMCOGA_CommonAttack::GetProjectileFragment(const uint8& InDamageIdx) const
{
	ISTRUE_N(nullptr != CurrentDefinition);
	ISTRUE_N(nullptr != CurrentDefinition->AttackTimingFragment);

	return CurrentDefinition->AttackTimingFragment->GetProjectileFragment(InDamageIdx);
}

void UMCOGA_CommonAttack::SetMovementOnBeginDamaging() const
{
	ensure(nullptr != CurrentDefinition);
	ensure(nullptr != CurrentDefinition->AttackTimingFragment);

	if (bAutoStopCharacter == CurrentDefinition->AttackTimingFragment->IsMovable(CurrentDamageTimingIdx))
	{
		StopCharacterFromMoving(false == CurrentDefinition->AttackTimingFragment->IsMovable(CurrentDamageTimingIdx));
	}

	if (false == CurrentDefinition->AttackTimingFragment->CanTurn(CurrentDamageTimingIdx))
	{
		StopCharacterFromTurning(true);
	}
}

void UMCOGA_CommonAttack::SetMovementOnEndDamaging() const
{
	ensure(nullptr != CurrentDefinition);
	ensure(nullptr != CurrentDefinition->AttackTimingFragment);

	if (bAutoStopCharacter == CurrentDefinition->AttackTimingFragment->IsMovable(CurrentDamageTimingIdx))
	{
		StopCharacterFromMoving(CurrentDefinition->AttackTimingFragment->IsMovable(CurrentDamageTimingIdx));
	}
	
	if (false == CurrentDefinition->AttackTimingFragment->CanTurn(CurrentDamageTimingIdx))
	{
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
	//MCOLOG_C(MCOTimer, TEXT("... Reset Timer "));

	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DamageByChannelTimerHandle);
}

float UMCOGA_CommonAttack::GetCurrentDamageBeginFrameCount() const
{
	ISTRUE_Z(nullptr != CurrentDefinition);
	ISTRUE_Z(nullptr != CurrentDefinition->AttackTimingFragment);
	return CurrentDefinition->AttackTimingFragment->GetDamageBeginTimeAfterPrevEndTime(CurrentDamageTimingIdx);
}

float UMCOGA_CommonAttack::GetCurrentDamageEndFrameCount() const
{
	ISTRUE_Z(nullptr != CurrentDefinition);
	ISTRUE_Z(nullptr != CurrentDefinition->AttackTimingFragment);
	return CurrentDefinition->AttackTimingFragment->GetDamageExistTime(CurrentDamageTimingIdx);
}

float UMCOGA_CommonAttack::GetCurrentDamageCheckRate() const
{
	ISTRUE_Z(nullptr != CurrentDefinition);
	ISTRUE_Z(nullptr != CurrentDefinition->AttackTimingFragment);
	return CurrentDefinition->AttackTimingFragment->GetDamageCheckRate(CurrentDamageTimingIdx);
}

void UMCOGA_CommonAttack::StartDamageBeginTimer()
{
	const float FrameCount = GetCurrentDamageBeginFrameCount();

	//MCOLOG_C(MCOTimer, TEXT("[%d] ... To Start : %f sec later "), CurrentDamageTimingIdx, FrameCount);
	
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

	//MCOLOG_C(MCOTimer, TEXT("[%d] ... To End : %f sec later "), CurrentDamageTimingIdx, FrameCount);
	
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
