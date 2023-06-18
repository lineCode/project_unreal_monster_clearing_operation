#include "MCOMontageDataDirectional.h"
#include "MCOActionFragment_Montage.h"


UAnimMontage* UMCOMontageDataDirectional::GetMontage(const float InDegree, bool bIsLog)
{
	const EMCOCharacterDirection Direction = GetDirectionFromDegree(InDegree);
	ensure(MontageFragments.Contains(Direction));

	if (true == bIsLog)
	{
		MCOLOG_C(MCOAbility, TEXT("Direction : %f == %s"), InDegree,
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
		MCOLOG_C(MCOAbility, TEXT("Direction : %f == %s"), InDegree,
			*FHelper::GetEnumDisplayName(TEXT("EMCOCharacterDirection"), (int64)Direction)
		);
	}
	
	return MontageFragments[Direction]->ActionDefinition->GetCollisionFragment();
}

EMCOCharacterDirection UMCOMontageDataDirectional:: GetDirectionFromDegree(const float InDegree) const
{
	const uint8 DirectionMax = static_cast<uint8>(EMCOCharacterDirection::Max);
	const float DegreeMax = 180.0f;
	const float Gap = DegreeMax / DirectionMax;
	
	for (int32 i = 0; i < DirectionMax / 2; i++)
	{
		const EMCOCharacterDirection Right = GetClosestDirectionFromDegree(InDegree + Gap * i); 
		const EMCOCharacterDirection Left = GetClosestDirectionFromDegree(InDegree - Gap * i);
		const bool HasRight = MontageFragments.Contains(Right);
		const bool HasLeft = MontageFragments.Contains(Left);
		
		if (true == HasRight)
		{
			return Right;	
		}
		else if (true == HasLeft)
		{
			return Left;
		}
	}

	return EMCOCharacterDirection::Front;
}

EMCOCharacterDirection UMCOMontageDataDirectional::GetClosestDirectionFromDegree(const float InDegree) const
{
	// Left
	if (InDegree < 0.0f)
	{
		if (InDegree > -22.5f)       return EMCOCharacterDirection::Front;
		else if (InDegree > -67.5f)  return EMCOCharacterDirection::Front_Left;
		else if (InDegree > -112.5f) return EMCOCharacterDirection::Left; 
		else if (InDegree > -157.5f) return EMCOCharacterDirection::Back_Left;
		else                         return EMCOCharacterDirection::Back;
	}
	// Right
	else
	{
		if (InDegree < 22.5f)        return EMCOCharacterDirection::Front;
		else if (InDegree < 67.5f)   return EMCOCharacterDirection::Front_Right;
		else if (InDegree < 112.5f)  return EMCOCharacterDirection::Right; 
		else if (InDegree < 157.5f)  return EMCOCharacterDirection::Back_Right;
		else                         return EMCOCharacterDirection::Back;
	}
}

