#include "MCOActionDefinition.h"
#include "MCOActionFragment_Collision.h"
#include "MCOActionFragment_Cooldown.h"
#include "MCOActionFragment_Damage.h"
#include "MCOActionFragment_Montage.h"
#include "MCOActionFragment_Stamina.h"
#include "MCOActionFragment_Timer.h"


const UMCOActionFragment_Collision* UMCOActionDefinition::GetCollisionFragment() const
{
	return Cast<UMCOActionFragment_Collision>(FindFragmentByClass(UMCOActionFragment_Collision::StaticClass()));
}

const UMCOActionFragment_Cooldown* UMCOActionDefinition::GetCooldownFragment() const
{
	return Cast<UMCOActionFragment_Cooldown>(FindFragmentByClass(UMCOActionFragment_Cooldown::StaticClass()));
}

const UMCOActionFragment_Damage* UMCOActionDefinition::GetDamageFragment() const
{
	return Cast<UMCOActionFragment_Damage>(FindFragmentByClass(UMCOActionFragment_Damage::StaticClass()));
}

const UMCOActionFragment_Montage* UMCOActionDefinition::GetMontageFragment() const
{
	return Cast<UMCOActionFragment_Montage>(FindFragmentByClass(UMCOActionFragment_Montage::StaticClass()));
}

const UMCOActionFragment_Timer* UMCOActionDefinition::GetTimerFragment() const
{
	return Cast<UMCOActionFragment_Timer>(FindFragmentByClass(UMCOActionFragment_Timer::StaticClass()));
}

const UMCOActionFragment_Stamina* UMCOActionDefinition::GetStaminaFragment() const
{
	return Cast<UMCOActionFragment_Stamina>(FindFragmentByClass(UMCOActionFragment_Stamina::StaticClass()));
}

const UMCOActionFragment* UMCOActionDefinition::FindFragmentByClass(TSubclassOf<UMCOActionFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UMCOActionFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
