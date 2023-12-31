#pragma once

#include "CoreMinimal.h"
#include "MCOActionData.h"
#include "MCOMontageDataDirectional.generated.h"

class UMCOActionDefinition;
class UMCOActionFragment_Montage;
class UMCOActionFragment_Collision;


UCLASS()
class MONSTERCO_API UMCOMontageDataDirectional : public UMCOActionData
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TMap<EMCOCharacterDirection, TObjectPtr<UMCOActionFragment_Montage>> MontageFragments;
	
public:
	void UpdateDirectionalDefinition(UMCOActionDefinition* OutDefinition, const EMCOCharacterDirection& InDirection = EMCOCharacterDirection::Max) const;
	EMCOCharacterDirection GetDirectionFromDegree(const float InDegree, const bool bLog = false) const;
	UAnimMontage* GetMontage(const EMCOCharacterDirection& InDirection);

protected:
	UMCOActionFragment_Cooldown* GetCooldownFragment(const EMCOCharacterDirection& InDirection) const;
	UMCOActionFragment_AttributeEffect* GetAttributeFragment(const EMCOCharacterDirection& InDirection) const;
	UMCOActionFragment_AttackTiming* GetAttackTimingFragment(const EMCOCharacterDirection& InDirection) const;
	UMCOActionFragment_Collision* GetCollisionFragment(const EMCOCharacterDirection& InDirection, const uint8& InDamageIdx) const;
	UMCOActionFragment_Montage* GetMontageFragment(const EMCOCharacterDirection& InDirection) const;
	float GetClosestDirectionFromDegree(float InDegree, EMCOCharacterDirection& OutDirection, const bool bLog) const;
};
