#include "MCOGameplayAbility_CommonAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/MCOAbilityTask_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/MCOCharacterTags.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Timer.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Damage.h"
#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"
#include "Physics/MCOPhysics.h"
#include "Interface/MCOCharacterInterface.h"
#include "Interface/MCOAttackedInterface.h"
#include "Interface/MCOHUDInterface.h"


UMCOGameplayAbility_CommonAttack::UMCOGameplayAbility_CommonAttack()
{
	// Class, Asset
	GETCLASS(AttributeEffect, UGameplayEffect, TEXT("/Game/AbilitySystem/GE_Attributes.GE_Attributes_C"));

	// Setting 
	bIsUsingCollision = false;
}

void UMCOGameplayAbility_CommonAttack::StartActivation_CommonAttack(UAnimMontage* InMontage, const FName& InSectionName,
	const UMCOActionFragment_Timer* InTimerFragment,
	const UMCOActionFragment_Damage* InDamageFragment,
	const UMCOActionFragment_Collision* InCollisionFragment)
{
	TimerFragment = InTimerFragment;
	DamageFragment = InDamageFragment;
	CollisionFragment = InCollisionFragment;
	
	DamagedCharacters.Reset();

	// Play Montage
	StartActivationWithMontage(InMontage, InSectionName);

	// Damage Timer
	SetDamageTimer();
}

void UMCOGameplayAbility_CommonAttack::OnTaskCompleted()
{
	Super::OnTaskCompleted();
	
	ResetTimer();
}

void UMCOGameplayAbility_CommonAttack::OnTaskCancelled()
{
	Super::OnTaskCancelled();
	
	ResetTimer();
}

void UMCOGameplayAbility_CommonAttack::BeginDamaging()
{
	StartDamageEndTimer();

	if (bIsUsingCollision)
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
	DamagedCharacters.Reset();
	StartDamageBeginTimer();
	
	if (bIsUsingCollision)
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
	AttackHitCheckByChannel();
}

void UMCOGameplayAbility_CommonAttack::BeginDamaging_Collision()
{
}

void UMCOGameplayAbility_CommonAttack::EndDamaging_Channel()
{
}

void UMCOGameplayAbility_CommonAttack::EndDamaging_Collision()
{
}

void UMCOGameplayAbility_CommonAttack::ApplyDamageAndStiffness(ACharacter* InAttackedCharacter)
{
	// Get ASC from AttackedCharacter
	IAbilitySystemInterface* AttackedCharacter = Cast<IAbilitySystemInterface>(InAttackedCharacter);
	ISTRUE(nullptr != AttackedCharacter);
	UAbilitySystemComponent* AttackedASC = AttackedCharacter->GetAbilitySystemComponent();
	ISTRUE(nullptr != AttackedASC);

	// Show Monster Widgets
	IMCOHUDInterface* HUDInterface = Cast<IMCOHUDInterface>(CurrentActorInfo->OwnerActor.Get());
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
		DamageFragment->Damage
	);
	HandleForAttributes.Data->SetSetByCallerMagnitude(
		FMCOCharacterTags::Get().GameplayEffect_StiffnessTag,
		DamageFragment->Stiffness
	);
	
	AttackedASC->ApplyGameplayEffectSpecToSelf(
		*HandleForAttributes.Data.Get(),
		AttackedASC->GetPredictionKeyForNewAction()
	);

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

float UMCOGameplayAbility_CommonAttack::CalculateTargetDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& DestLocation, bool bLog) const
{
	const FVector Source = FVector(SourceLocation.X, SourceLocation.Y, 0.0f);
	const FVector Dest = FVector(DestLocation.X, DestLocation.Y, 0.0f);
	
	const FVector ForwardVector = FVector(SourceForward.X, SourceForward.Y, 0.0f);
	const FVector LookVector = (Dest - Source).GetSafeNormal();
	
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
		MCOLOG(TEXT("Degree : %f"), Degree);
	}
	
	return Degree;	
}

float UMCOGameplayAbility_CommonAttack::CalculateDamagedDegree(const FVector& TargetLocation, const FVector& TargetForward, const FVector& AttackDirection, bool bLog) const
{		
	const FVector ForwardVector = FVector(TargetForward.X, TargetForward.Y, 0.0f);
	const FVector LookVector = FVector(AttackDirection.X, AttackDirection.Y, 0.0f);
	
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
		MCOLOG(TEXT("Degree : %f"), Degree);
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugLine(
		GetWorld(),
		TargetLocation,
		TargetLocation + ForwardVector * 1000.0f,
		FColor::Red,
		false,
		2.0f
	);
	
	DrawDebugLine(
		GetWorld(),
		TargetLocation,
		TargetLocation + LookVector * 1000.0f,
		FColor::Blue,
		false,
		2.0f
	);
#endif
	
	return Degree;
}

void UMCOGameplayAbility_CommonAttack::SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const
{
	IMCOCharacterInterface* CharacterInterface = Cast<IMCOCharacterInterface>(InAttackedCharacter);
	ISTRUE(CharacterInterface);
	CharacterInterface->SetDamagedData(CurrentDamagedData);
}

FVector UMCOGameplayAbility_CommonAttack::GetDirectionVector(const EMCOCharacterDirection& InAttackDirection, ACharacter* InCharacter) const
{
	FVector Direction;
	if (nullptr == InCharacter)
	{
		return Direction;
	}
	
	switch (InAttackDirection)
	{
	case EMCOCharacterDirection::Front:
	{
		Direction = InCharacter->GetActorForwardVector();
	}
	break;
	case EMCOCharacterDirection::Back:
	{
		Direction = -InCharacter->GetActorForwardVector();
	}
	break;
	case EMCOCharacterDirection::Right:
	{
		Direction = InCharacter->GetActorRightVector();
	}
	break;
	case EMCOCharacterDirection::Left:
	{
		Direction = -InCharacter->GetActorRightVector();
	}
	break;
	case EMCOCharacterDirection::Front_Left:
	{
		Direction = (InCharacter->GetActorForwardVector() + -InCharacter->GetActorRightVector()).GetSafeNormal();
	}
	break;
	case EMCOCharacterDirection::Front_Right:
	{
		Direction = (InCharacter->GetActorForwardVector() + InCharacter->GetActorRightVector()).GetSafeNormal();
	}
	break;
	case EMCOCharacterDirection::Back_Left:
	{
		Direction = (-InCharacter->GetActorForwardVector() + -InCharacter->GetActorRightVector()).GetSafeNormal();
	}
	break;
	case EMCOCharacterDirection::Back_Right:
	{
		Direction = (-InCharacter->GetActorForwardVector() + InCharacter->GetActorRightVector()).GetSafeNormal();
	}
	break;
	default:
	{
		Direction = InCharacter->GetActorForwardVector();
	}
	break;
	}

	return Direction;
}

void UMCOGameplayAbility_CommonAttack::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter)
{
	ISTRUE(nullptr != InAttacker);
	ISTRUE(nullptr != InAttackCauser);
	ISTRUE(nullptr != InAttackedCharacter);
	
	ISTRUE(false == DamagedCharacters.Contains(InAttackedCharacter));

	IMCOAttackedInterface* AttackedInterface = Cast<IMCOAttackedInterface>(InAttackedCharacter);
	ISTRUE(nullptr != AttackedInterface);  // need to add attachments.....
	
	DamagedCharacters.Emplace(InAttackedCharacter);
			
	CurrentDamagedData.AttackedDegree = CalculateDamagedDegree(
		InAttackedCharacter->GetActorLocation(),
		InAttackedCharacter->GetActorForwardVector(),
		-GetDirectionVector(CollisionFragment->AttackDirection, InAttacker)
	);
	
	// CurrentDamagedData.AttackedDegree = CalculateTargetDegree(
	// 	InAttackedCharacter->GetActorLocation(),
	// 	InAttackedCharacter->GetActorForwardVector(),
	// 	InAttackCauser->GetActorLocation()
	// );
	// CurrentDamagedData.AttackedDirection = CalculateLookVector(
	// InAttackCauser->GetActorLocation(),
	// InAttackedCharacter->GetActorLocation()
	// );
	
	SendDamagedDataToTarget(InAttackedCharacter);
	ApplyDamageAndStiffness(InAttackedCharacter);
}

void UMCOGameplayAbility_CommonAttack::AttackHitCheckByChannel()
{
	ACharacter* Attacker = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	ISTRUE(nullptr != Attacker);

	TArray<FHitResult> OutHitResults;
	const FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Attack),
		false,
		Attacker
	);
	
	IMCOCharacterInterface* AttackerInterface = Cast<IMCOCharacterInterface>(Attacker);
	ISTRUE(nullptr != AttackerInterface);
	const float Radius = AttackerInterface->GetCapsuleRadius();

	const FVector AttackDirection = GetDirectionVector(CollisionFragment->AttackDirection, Attacker);
	
	const FVector Start = Attacker->GetActorLocation() +
		(Attacker->GetActorForwardVector() * Radius) +
		(Attacker->GetActorForwardVector() * CollisionFragment->AdditiveLocationFromFront);
	
	const FVector End = Start + AttackDirection * CollisionFragment->AttackLength;
	
	const FCollisionShape Shape = FCollisionShape::MakeSphere(CollisionFragment->AttackRadius);
	
	const bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CHANNEL_MCOACTION_TRACE,
		Shape,
		Params
	);
	
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
		
		
#if ENABLE_DRAW_DEBUG
		DrawDebug(AttackDirection, Start, End, HitDetected);
#endif
		
		DamagedCharacters.Emplace(AttackedCharacter);
			
		CurrentDamagedData.AttackedDegree = CalculateDamagedDegree(
			AttackedCharacter->GetActorLocation(),
			AttackedCharacter->GetActorForwardVector(),
			-AttackDirection
		);

		// CurrentDamagedData.AttackedDegree = CalculateDegree(
		// 	AttackedCharacter->GetActorLocation(),
		// 	AttackedCharacter->GetActorForwardVector(),
		// 	End,
		// 	true
		// );
		// CurrentDamagedData.AttackedDirection = CalculateLookVector(
		// End,
		// AttackedCharacter->GetActorLocation()
		// );
			
		SendDamagedDataToTarget(AttackedCharacter);
		ApplyDamageAndStiffness(AttackedCharacter);
	}

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
		5.0f 
	);
}

void UMCOGameplayAbility_CommonAttack::SetDamageTimer()
{
	CurrentDamageTimingIdx = 0;
	StartDamageBeginTimer();
}

void UMCOGameplayAbility_CommonAttack::ResetTimer()
{
	DamageTimerHandle.Invalidate();
}

float UMCOGameplayAbility_CommonAttack::GetCurrentDamageBeginFrameCount() const
{
	return TimerFragment->GetDamageBeginTimeAfterPrevEndTime(CurrentDamageTimingIdx);
}

float UMCOGameplayAbility_CommonAttack::GetCurrentDamageEndFrameCount() const
{
	return TimerFragment->GetDamageExistTime(CurrentDamageTimingIdx);
}

void UMCOGameplayAbility_CommonAttack::StartDamageBeginTimer()
{
	ResetTimer();
	
	ISTRUE(CurrentDamageTimingIdx < TimerFragment->DamageTimings.Num());

	const float FrameCount = GetCurrentDamageBeginFrameCount();
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
	ResetTimer();
	
	ISTRUE(CurrentDamageTimingIdx < TimerFragment->DamageTimings.Num());
	
	const float FrameCount = GetCurrentDamageEndFrameCount();
	ISTRUE(FrameCount > 0.0f);
	
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ThisClass::EndDamaging,
		FrameCount,
		false
	);
	
	CurrentDamageTimingIdx++;
}
//virtual FActiveGameplayEffectHandle UAbilitySystemComponent::ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey = FPredictionKey());
//virtual FActiveGameplayEffectHandle UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect,	FPredictionKey PredictionKey = FPredictionKey());
