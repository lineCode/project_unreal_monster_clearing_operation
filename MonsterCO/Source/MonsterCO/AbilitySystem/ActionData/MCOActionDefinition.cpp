#include "MCOActionDefinition.h"
// #include "MCOActionFragment_Collision.h"
// #include "MCOActionFragment_Cooldown.h"
// #include "MCOActionFragment_Montage.h"
// #include "MCOActionFragment_Attribute.h"
// #include "MCOActionFragment_AttackTiming.h"


// const UMCOActionFragment_Collision* UMCOActionDefinition::GetCollisionFragment() const
// {
// 	return Cast<UMCOActionFragment_Collision>(FindFragmentByClass(UMCOActionFragment_Collision::StaticClass()));
// }
//
// const UMCOActionFragment_Cooldown* UMCOActionDefinition::GetCooldownFragment() const
// {
// 	return Cast<UMCOActionFragment_Cooldown>(FindFragmentByClass(UMCOActionFragment_Cooldown::StaticClass()));
// }
//
// const UMCOActionFragment_Montage* UMCOActionDefinition::GetMontageFragment() const
// {
// 	return Cast<UMCOActionFragment_Montage>(FindFragmentByClass(UMCOActionFragment_Montage::StaticClass()));
// }
//
// const UMCOActionFragment_AttackTiming* UMCOActionDefinition::GetTimerFragment() const
// {
// 	return Cast<UMCOActionFragment_AttackTiming>(FindFragmentByClass(UMCOActionFragment_AttackTiming::StaticClass()));
// }
//
// const UMCOActionFragment_Attribute* UMCOActionDefinition::GetAttributeFragment() const
// {
// 	return Cast<UMCOActionFragment_Attribute>(FindFragmentByClass(UMCOActionFragment_Attribute::StaticClass()));
// }
//
// const UMCOActionFragment* UMCOActionDefinition::FindFragmentByClass(TSubclassOf<UMCOActionFragment> FragmentClass) const
// {
// 	if (FragmentClass != nullptr)
// 	{
// 		for (UMCOActionFragment* Fragment : Fragments)
// 		{
// 			if (Fragment && Fragment->IsA(FragmentClass))
// 			{
// 				return Fragment;
// 			}
// 		}
// 	}
//
// 	return nullptr;
// }
