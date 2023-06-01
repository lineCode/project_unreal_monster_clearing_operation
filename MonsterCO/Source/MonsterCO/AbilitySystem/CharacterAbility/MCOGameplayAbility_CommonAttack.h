#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOGameplayAbility_CommonAttack.generated.h"

class UGameplayEffect;
class UMCOMontageData;
class UMCOAttackFragment_Timer;
class UMCOAttackFragment_Damage;
class UMCOAttackFragment_Collision;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_CommonAttack : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_CommonAttack();

// --- Ability
protected:
	void StartActivation_CommonAttack(UAnimMontage* InMontage, const FName& InSectionName,
		UMCOAttackFragment_Timer* InTimerFragment,
		UMCOAttackFragment_Damage* InDamageFragment,
		UMCOAttackFragment_Collision* InCollisionFragment
	);

	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;
	void BeginDamaging();
	void EndDamaging();

	virtual void BeginDamaging_Channel();
	virtual void BeginDamaging_Collision();
	virtual void EndDamaging_Channel();
	virtual void EndDamaging_Collision();

// --- Attack
protected:
	void ApplyDamageAndStiffness(ACharacter* InAttackedCharacter);
	float CalculateTargetDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& DestLocation, bool bLog = false) const;
	float CalculateDamagedDegree(const FVector& TargetLocation, const FVector& TargetForward, const FVector& AttackDirection, bool bLog = false) const;
	void SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const;
	FVector GetDirectionVector(const EMCOCharacterDirection& InAttackDirection, ACharacter* InCharacter) const;

	UFUNCTION()
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter);

	void AttackHitCheckByChannel();
	void DrawDebug(const FVector& AttackForward, const FVector& Start, const FVector& End, bool bHitDetected) const;
	
protected:
	UPROPERTY(EditAnywhere, Category = "MCO|Effect")
	TSubclassOf<UGameplayEffect> AttributeEffect;
	

// --- Damage Timer
protected:
	void SetDamageTimer();
	void ResetTimer();
	
private:
	float GetCurrentDamageBeginFrameCount();
	float GetCurrentDamageEndFrameCount();
	void StartDamageBeginTimer();
	void StartDamageEndTimer();
	
protected:
	UPROPERTY(EditAnywhere, Category = "MCO|AttackStyle")
	uint8 bIsUsingCollision : 1;
	
	UPROPERTY()
	TObjectPtr<UMCOAttackFragment_Damage> DamageFragment;

	UPROPERTY()
	TObjectPtr<UMCOAttackFragment_Collision> CollisionFragment;
	
	UPROPERTY()
	TObjectPtr<UMCOAttackFragment_Timer> TimerFragment;
	
	
private:
	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> DamagedCharacters;
	
	int32 CurrentDamageTimingIdx = 0;
	FTimerHandle DamageTimerHandle;
	FMCODamagedData CurrentDamagedData;
	
};
