#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MCOCommonMontageData.generated.h"


USTRUCT(BlueprintType)
struct FMCOCommonSkillData
{
	GENERATED_BODY()

public:
	bool CanApplyCooldown() const { return (CooldownTime != 0.0f && CooldownTags.IsEmpty() == false); }
	
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.0f;
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer CooldownTags = FGameplayTagContainer();
};

USTRUCT(BlueprintType)
struct FMCOGrantedAttributeValues
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float StiffnessValue = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float DamageValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FMCODamageTiming
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float DamageBeginFrameCount = 0.0f;
		
	UPROPERTY(EditAnywhere)
	float DamageEndFrameCount = 0.0f;
};

USTRUCT(BlueprintType)
struct FMCOCollisionData
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


UCLASS(BlueprintType)
class MONSTERCO_API UMCOCommonMontageData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMCOCommonMontageData();

	float GetDamageBeginFrameCountAfterEndTime(uint8 CurrentIdx, float AttackSpeedRate = 1.0f);
	float GetDamageExistFrameCount(uint8 CurrentIdx, float AttackSpeedRate = 1.0f);

protected:
	float GetFrameCountByConsideringSpeed(float FrameCount, float AttackSpeedRate = 1.0f);
		
public:
	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> MontageToPlay;
	
	UPROPERTY(EditAnywhere, Category = Montage)
	float FrameRate;
		
	UPROPERTY(EditAnywhere, Category = Montage)
	FName MontageSectionName;
	
	UPROPERTY(EditAnywhere, Category = SkillData)
	FMCOCommonSkillData CommonSkillData;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
	FMCOGrantedAttributeValues AttributeValues;
	
	UPROPERTY(EditAnywhere, Category = Attack)
	FMCOCollisionData CollisionData;

	UPROPERTY(EditAnywhere, Category = DamageTiming)
	TArray<FMCODamageTiming> DamageTimings;
};
