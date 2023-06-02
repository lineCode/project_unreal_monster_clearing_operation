#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOMontageDataDirectional.generated.h"

class UMCOActionDefinition;
class UMCOActionFragment_Montage;
class UMCOActionFragment_Collision;


UCLASS()
class MONSTERCO_API UMCOMontageDataDirectional : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FGameplayTag ActivationTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	EMCOCharacterDirectionOption Option;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TMap<EMCOCharacterDirection, TObjectPtr<UMCOActionFragment_Montage>> MontageFragments;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Definition, Instanced)
	TObjectPtr<UMCOActionDefinition> ActionDefinition;
	
public:
	UAnimMontage* GetMontage(const float InDegree, bool bIsLog = false);
	const UMCOActionFragment_Collision* GetCollisionFragment(const float InDegree, bool bIsLog = false);
	
protected:
	EMCOCharacterDirection GetDirectionFromDegree(const float InDegree) const;
};
