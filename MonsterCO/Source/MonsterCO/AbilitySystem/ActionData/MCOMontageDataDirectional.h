#pragma once

#include "MonsterCO.h"
#include "MCOMontageDataBase.h"
#include "MCOMontageDataDirectional.generated.h"


class UMCOAttackFragment_Damage;
class UMCOAttackFragment_Cooldown;
class UMCOAttackFragment_Timer;
class UMCOAttackFragment_Collision;

USTRUCT(BlueprintType)
struct FMCOMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage = nullptr;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Collision> CollisionFragment = nullptr;
};



UCLASS()
class MONSTERCO_API UMCOMontageDataDirectional : public UMCOMontageDataBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	EMCOCharacterDirectionOption Option;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TMap<EMCOCharacterDirection, FMCOMontageData> Montages;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Cooldown> CooldownFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Damage> DamageFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Timer> TimerFragment;
	
public:
	UAnimMontage* GetMontage(const float InDegree, bool bIsLog = false);
	UMCOAttackFragment_Collision* GetCollisionFragment(const float InDegree, bool bIsLog = false);
	
protected:
	EMCOCharacterDirection GetDirectionFromDegree(const float InDegree) const;
};
