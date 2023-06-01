#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCOMontageDataUnit.generated.h"

class UMCOAttackFragment_Damage;
class UMCOAttackFragment_Cooldown;
class UMCOAttackFragment_Timer;
class UMCOAttackFragment_Collision;


// this class is for combo montage
UCLASS()
class MONSTERCO_API UMCOMontageDataUnit : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> Montage;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Cooldown> CooldownFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Damage> DamageFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Collision> CollisionFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fragment, Instanced)
	TObjectPtr<UMCOAttackFragment_Timer> TimerFragment;
	
public:
	UAnimMontage* GetMontage();
};
