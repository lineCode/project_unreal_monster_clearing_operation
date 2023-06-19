#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOGameplayAbility_CommonAttack.generated.h"

class UGameplayEffect;
class UMCOActionFragment_AttackTiming;
class UMCOActionFragment_Collision;


UCLASS()
class MONSTERCO_API UMCOGameplayAbility_CommonAttack : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_CommonAttack();

	
// --- Ability
protected:
	void StartActivation_CommonAttack(UAnimMontage* InMontage, const FName& InSectionName);

	virtual void OnTaskCompleted() override;
	virtual void OnTaskCancelled() override;
	
	void BeginDamaging();
	void EndDamaging();

private:
	void BeginDamaging_Channel();
	void EndDamaging_Channel();
	void BeginDamaging_Collision();
	void EndDamaging_Collision();

// --- Attack
protected:
	void ApplyDamageAndStiffness(ACharacter* InAttackedCharacter);
	float CalculateDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& TargetDirection, bool bLog = false) const;
	void SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const;

// --- Attack/Overlap
	UFUNCTION()
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter, const FHitResult& SweepResult);

// --- Attack/Channel
	void AttackHitCheckByChannel();
	void DrawDebug(const FVector& AttackForward, const FVector& Start, const FVector& End, bool bHitDetected) const;
	
protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> AttributeEffect;
	

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
