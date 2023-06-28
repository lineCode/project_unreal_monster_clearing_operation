#pragma once

#include "MonsterCO.h"
#include "AbilitySystem/CharacterAbility/MCOGameplayAbility.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOGA_CommonAttack.generated.h"



class UGameplayEffect;
class UMCOActionFragment_AttackTiming;
class UMCOActionFragment_Collision;



UCLASS()
class MONSTERCO_API UMCOGA_CommonAttack : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGA_CommonAttack();
	
	
// --- Ability
protected:
	void StartAttackActivation(UAnimMontage* InMontage, const FName& InSectionName);

	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;
	
	void BeginDamaging();
	void EndDamaging();

private:
	void BeginDamaging_Channel();
	void EndDamaging_Channel();
	void BeginDamaging_Collision();
	void EndDamaging_Collision();

protected:
	UPROPERTY(EditAnywhere, Category = "MCO|AttackStyle")
	uint8 bIsInstantAttack : 1;
	
// --- Attack
protected:
	void ApplyDamageAndStiffness(ACharacter* InAttackedCharacter);
	float CalculateDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& TargetDirection, bool bLog = false) const;
	void SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const;

	void Attack();
	
// --- Attack/Overlap
	UFUNCTION()
	void OnCollisionBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter, const FHitResult& SweepResult);

// --- Attack/Channel
	void AttackByProjectile();
	void AttackByInstantCheck();
	void DrawDebug(const FVector& AttackForward, const FVector& Start, const FVector& End, bool bHitDetected) const;
	
protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> InstantEffectWithCue;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DurationEffectWithCue;
	
// --- Movement
protected:
	void SetMovementOnBeginDamaging() const;
	void SetMovementOnEndDamaging() const;

	
// --- Damage Timer
protected:
	void SetDamageTimer();
	void ResetDamageTimer();
	
private:
	float GetCurrentDamageBeginFrameCount() const;
	float GetCurrentDamageEndFrameCount() const;
	float GetCurrentDamageCheckRate() const;
	void StartDamageBeginTimer();
	void StartDamageEndTimer();
	
protected:
	UPROPERTY(EditAnywhere, Category = "MCO|AttackStyle")
	uint8 bUseOverlapEvent : 1;
	
private:
	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> DamagedCharacters;
	
	int32 CurrentDamageTimingIdx = 0;
	FTimerHandle DamageTimerHandle;
	FMCODamagedData CurrentDamagedData;
	
	FTimerHandle DamageByChannelTimerHandle;
	
};
