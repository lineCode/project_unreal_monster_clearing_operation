#pragma once

#include "CoreMinimal.h"
#include "MCOActionData.h"
#include "MCOMontageDataCombo.generated.h"

class UMCOActionFragment_Montage;

UCLASS()
class MONSTERCO_API UMCOMontageDataCombo : public UMCOActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	FName MontageSectionName = NAME_None;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<TObjectPtr<UMCOActionFragment_Montage>> MontageFragments;
	
public:
	UAnimMontage* GetMontage(const uint8& InComboIdx);
	uint8 GetMaxCombo() const;
	void UpdateComboDefinition(UMCOActionDefinition* OutDefinition, const uint8& InComboIdx = 0) const;

protected:
	UMCOActionFragment_Cooldown* GetCooldownFragment(const uint8& InComboIdx) const;
	UMCOActionFragment_Attribute* GetAttributeFragment(const uint8& InComboIdx) const;
	UMCOActionFragment_AttackTiming* GetAttackTimingFragment(const uint8& InComboIdx) const;
	UMCOActionFragment_Collision* GetCollisionFragment(const uint8& InComboIdx) const;
	UMCOActionFragment_Montage* GetMontageFragment(const uint8& InComboIdx) const;
	
};
