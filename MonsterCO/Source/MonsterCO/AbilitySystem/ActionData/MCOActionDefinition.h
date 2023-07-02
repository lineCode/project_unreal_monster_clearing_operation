#pragma once

#include "MonsterCO.h"
#include "MCOActionDefinition.generated.h"


class UMCOActionFragment_Collision;
class UMCOActionFragment_Cooldown;
class UMCOActionFragment_AttributeEffect;
class UMCOActionFragment_AttackTiming;


UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class MONSTERCO_API UMCOActionFragment : public UObject
{
	GENERATED_BODY()
};


UCLASS(DefaultToInstanced, EditInlineNew)
class MONSTERCO_API UMCOActionDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	// Cool down
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_Cooldown> CooldownFragment;

	// Stamina, Item effect ... etc 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_AttributeEffect> AttributeFragment;

	// Attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_AttackTiming> AttackTimingFragment;

	// Common Collision info 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_Collision> CollisionFragment;
	
	
// protected:
// 	const UMCOActionFragment* FindFragmentByClass(TSubclassOf<UMCOActionFragment> FragmentClass) const;
	
};
