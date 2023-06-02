#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.generated.h"

class UMCOActionFragment_Collision;
class UMCOActionFragment_Cooldown;
class UMCOActionFragment_Damage;
class UMCOActionFragment_Montage;
class UMCOActionFragment_Timer;



UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class MONSTERCO_API UMCOActionFragment : public UObject
{
	GENERATED_BODY()
};


UCLASS(DefaultToInstanced, EditInlineNew)
class MONSTERCO_API UMCOActionDefinition : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TSet<TObjectPtr<UMCOActionFragment>> Fragments;

public:
	const UMCOActionFragment_Collision* GetCollisionFragment() const;
	const UMCOActionFragment_Cooldown* GetCooldownFragment() const;
	const UMCOActionFragment_Damage* GetDamageFragment() const;
	const UMCOActionFragment_Montage* GetMontageFragment() const;
	const UMCOActionFragment_Timer* GetTimerFragment() const;

protected:
	const UMCOActionFragment* FindFragmentByClass(TSubclassOf<UMCOActionFragment> FragmentClass) const;
	
};
