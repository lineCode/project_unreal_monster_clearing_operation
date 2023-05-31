#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOActionData.generated.h"



USTRUCT(BlueprintType)
struct FMCOAttackAttributes
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float StiffnessValue = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float DamageValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FMCODamageFrameCounts
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float DamageBeginFrameCount = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float DamageEndFrameCount = 0.0f;
};

USTRUCT(BlueprintType)
struct FMCOAttackCollisionData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EMCOCharacterDirection AttackDirection = EMCOCharacterDirection::Front;
	
	UPROPERTY(EditAnywhere)
	FVector AdditiveLocationFromFront = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere)
	float AttackLength = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float AttackRadius = 0.0f;
	
	UPROPERTY(EditAnywhere)
	FName SocketName = NAME_None;
};


USTRUCT(BlueprintType)
struct FAttackActionData
{
	GENERATED_BODY()
	
// --- Montage
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	float MontageFrameRate = 30.0f;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;

// --- Damage 
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	FMCOAttackAttributes AttackAttributes = FMCOAttackAttributes();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	FMCOAttackCollisionData AttackCollisionData = FMCOAttackCollisionData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	TArray<FMCODamageFrameCounts> AttackDamageTimings = TArray<FMCODamageFrameCounts>();

// --- Combo
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combo)
	float NextComboFrameCount = 0.0f;
};


UCLASS()
class MONSTERCO_API UMCOActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

// --- Tag is everything
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Tag)
	FGameplayTag ActivationTag = FGameplayTag();
	
// --- Cooldown
public:
	bool CanApplyCooldown() const { return (CooldownTime != 0.0f && CooldownTags.IsEmpty() == false); }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooldown)
	float CooldownTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cooldown)
	FGameplayTagContainer CooldownTags = FGameplayTagContainer();

// --- If it's attack
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack)
	TArray<FAttackActionData> AttackActionDatas;

public:
	UAnimMontage* GetMontage(uint8 InComboIdx = 0);
	float GetFrameRate(uint8 InComboIdx = 0);
	const FMCOAttackAttributes GetAttackAttributes(uint8 InComboIdx = 0);
	const FMCOAttackCollisionData GetCollisionData(uint8 InComboIdx = 0);
	const FMCODamageFrameCounts GetDamageTimingFrameCount(uint8 InDamageIdx, uint8 InComboIdx = 0);

public:
	float GetDamageBeginFrameCountAfterEndTime(uint8 InDamageIdx, uint8 InComboIdx = 0, float AttackSpeedRate = 1.0f);
	float GetDamageExistFrameCount(uint8 InDamageIdx, uint8 InComboIdx = 0, float AttackSpeedRate = 1.0f);

protected:
	float GetFrameCountByConsideringSpeed(float FrameCount, uint8 InComboIdx = 0, float AttackSpeedRate = 1.0f);

// --- If it's combo attack
public:
	uint8 GetMaxCombo() const { return AttackActionDatas.Num(); }
	float GetComboFrameCount(uint8 InComboIdx) const;
	float GetComboCheckTime(uint8 InComboIdx, float AttackSpeedRate = 1.0f);
	
};
