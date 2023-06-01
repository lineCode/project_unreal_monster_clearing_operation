#pragma once

#include "CoreMinimal.h"
#include "MCOMontageDataBase.h"
#include "MCOMontageDataSingle.generated.h"


class UMCOAttackFragment_Damage;
class UMCOAttackFragment_Cooldown;
class UMCOAttackFragment_Timer;
class UMCOAttackFragment_Collision;
UCLASS()
class MONSTERCO_API UMCOMontageDataSingle : public UMCOMontageDataBase
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
