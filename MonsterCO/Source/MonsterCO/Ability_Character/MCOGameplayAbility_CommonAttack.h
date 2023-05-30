#pragma once

#include "MonsterCO.h"
#include "MCOGameplayAbility.h"
#include "Character/MCOCommonMontageData.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOGameplayAbility_CommonAttack.generated.h"

class UGameplayEffect;

UCLASS()
class MONSTERCO_API UMCOGameplayAbility_CommonAttack : public UMCOGameplayAbility
{
	GENERATED_BODY()

public:
	UMCOGameplayAbility_CommonAttack();

// --- Ability
protected:
	void StartActivation_CommonAttack(UMCOCommonMontageData* InMontageData);

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
	void ApplyDamageAndStiffness(const FMCOGrantedAttributeValues& InAttributeValues, ACharacter* InAttackedCharacter);
	float CalculateTargetDegree(const FVector& SourceLocation, const FVector& SourceForward, const FVector& DestLocation, bool bLog = false) const;
	float CalculateDamagedDegree(const FVector& TargetLocation, const FVector& TargetForward, const FVector& AttackDirection, bool bLog = false) const;
	void SendDamagedDataToTarget(ACharacter* InAttackedCharacter) const;
	FVector GetDirectionVector(const EMCOCharacterDirection& InAttackDirection, ACharacter* InCharacter) const;

	UFUNCTION()
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InAttackedCharacter);

	void AttackHitCheckByChannel(const FMCOCollisionData& InCollisionData, const FMCOGrantedAttributeValues& InAttributeValues);
	void DrawDebug(const FVector& AttackForward, const FVector& Start, const FVector& End, const FMCOCollisionData& InCollisionByChannelData, bool bHitDetected) const;
	
protected:
	UPROPERTY(EditAnywhere, Category = Effect)
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
	UPROPERTY(EditAnywhere, Category = AttackStyle)
	uint8 bIsUsingCollision : 1;

	UPROPERTY()
	TObjectPtr<UMCOCommonMontageData> CurrentData;
	
private:
	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> DamagedCharacters;
	
	int32 CurrentDamageTimingIdx = 0;
	FTimerHandle DamageTimerHandle;
	FMCODamagedData CurrentDamagedData;
	
};
