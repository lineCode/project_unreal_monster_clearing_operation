#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.generated.h"

class UMCOActionFragment_Collision;
class UMCOActionFragment_Cooldown;
class UMCOActionFragment_Montage;
class UMCOActionFragment_AttackTiming;
class UMCOActionFragment_Attribute;


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
	const UMCOActionFragment_Montage* GetMontageFragment() const;
	const UMCOActionFragment_AttackTiming* GetTimerFragment() const;
	const UMCOActionFragment_Attribute* GetAttributeFragment() const;

protected:
	const UMCOActionFragment* FindFragmentByClass(TSubclassOf<UMCOActionFragment> FragmentClass) const;
	
};
