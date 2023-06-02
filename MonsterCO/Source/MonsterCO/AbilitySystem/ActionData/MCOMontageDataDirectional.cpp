#include "MCOMontageDataDirectional.h"
#include "MCOActionFragment_Montage.h"


UAnimMontage* UMCOMontageDataDirectional::GetMontage(const float InDegree, bool bIsLog)
{
	const EMCOCharacterDirection Direction = GetDirectionFromDegree(InDegree);
	ensure(MontageFragments.Contains(Direction));

	if (true == bIsLog)
	{
		MCOLOG(TEXT("%f : %s"), InDegree,
			*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)Direction)
		);
	}
	
	return MontageFragments[Direction]->GetMontage();
}

const UMCOActionFragment_Collision* UMCOMontageDataDirectional::GetCollisionFragment(const float InDegree, bool bIsLog)
{
	const EMCOCharacterDirection Direction = GetDirectionFromDegree(InDegree);
	ensure(MontageFragments.Contains(Direction));
	ensure(ActionDefinition);
	
	if (true == bIsLog)
	{
		MCOLOG(TEXT("%f : %s"), InDegree,
			*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)Direction)
		);
	}
	
	return MontageFragments[Direction]->ActionDefinition->GetCollisionFragment();
}

EMCOCharacterDirection UMCOMontageDataDirectional::GetDirectionFromDegree(const float InDegree) const
{
	switch(Option)
	{
	case EMCOCharacterDirectionOption::FrontBack:
	{
		if (-90.0f < InDegree && InDegree < 90.0f)
		{
			return EMCOCharacterDirection::Front; 
		}
		else
		{
			return EMCOCharacterDirection::Back; 
		}
	}
	break;
	case EMCOCharacterDirectionOption::LeftRight:
	{
		if (InDegree < 0.0f)
		{
			return EMCOCharacterDirection::Left; 
		}
		else
		{
			return EMCOCharacterDirection::Right; 
		}
	}
	break;
	case EMCOCharacterDirectionOption::FourSide:
	{
		if (InDegree < 0.0f)
		{
			if (InDegree > -45.0f)       return EMCOCharacterDirection::Front;
			else if (InDegree > -135.0f)  return EMCOCharacterDirection::Left; 
			else                         return EMCOCharacterDirection::Back;
		}
		else
		{
			if (InDegree < 45.0f)        return EMCOCharacterDirection::Front;
			else if (InDegree < 135.0f)   return EMCOCharacterDirection::Right; 
			else                         return EMCOCharacterDirection::Back;
		}
	}
	break;
	case EMCOCharacterDirectionOption::SixSide:
	{
		// Left
		if (InDegree < 0.0f)
		{
			if (InDegree > -60.0f)         return EMCOCharacterDirection::Front_Left;
			else if (InDegree > -120.0f)   return EMCOCharacterDirection::Left; 
			else                           return EMCOCharacterDirection::Back_Left;
		}
		// Right
		else
		{
			if (InDegree < 60.0f)          return EMCOCharacterDirection::Front_Right;
			else if (InDegree < 120.0f)    return EMCOCharacterDirection::Right; 
			else                           return EMCOCharacterDirection::Back_Right;
		}
	}
	break;
	case EMCOCharacterDirectionOption::EightSide:
	{
		// Left
		if (InDegree < 0.0f)
		{
			if (InDegree > -30.0f)       return EMCOCharacterDirection::Front;
			else if (InDegree > -60.0f)  return EMCOCharacterDirection::Front_Left;
			else if (InDegree > -90.0f)  return EMCOCharacterDirection::Left; 
			else if (InDegree > -120.0f) return EMCOCharacterDirection::Back_Left;
			else                         return EMCOCharacterDirection::Back;
		}
		// Right
		else
		{
			if (InDegree < 30.0f)        return EMCOCharacterDirection::Front;
			else if (InDegree < 60.0f)   return EMCOCharacterDirection::Front_Right;
			else if (InDegree < 90.0f)   return EMCOCharacterDirection::Right; 
			else if (InDegree < 120.0f)  return EMCOCharacterDirection::Back_Right;
			else                         return EMCOCharacterDirection::Back;
		}
	}
	break;
	}
	
	return EMCOCharacterDirection::Front;
}

