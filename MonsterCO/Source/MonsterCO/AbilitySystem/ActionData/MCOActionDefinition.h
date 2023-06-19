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
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_Cooldown> CooldownFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_Attribute> AttributeFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_AttackTiming> AttackTimingFragment;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionFragment_Collision> CollisionFragment;
	
// protected:
// 	const UMCOActionFragment* FindFragmentByClass(TSubclassOf<UMCOActionFragment> FragmentClass) const;
	
};
