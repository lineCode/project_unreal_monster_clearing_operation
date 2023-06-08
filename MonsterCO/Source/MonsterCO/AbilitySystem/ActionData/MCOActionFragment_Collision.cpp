#include "AbilitySystem/ActionData/MCOActionFragment_Collision.h"


FVector UMCOActionFragment_Collision::GetAttackDirection(const AActor* InActor) const
{
	FVector Direction;
	if (nullptr == InActor)
	{
		return Direction;
	}
	
	switch (AttackDirection)
	{
	case EMCOCharacterDirection::Front:
		{
			Direction = InActor->GetActorForwardVector();
		}
		break;
	case EMCOCharacterDirection::Back:
		{
			Direction = -InActor->GetActorForwardVector();
		}
		break;
	case EMCOCharacterDirection::Right:
		{
			Direction = InActor->GetActorRightVector();
		}
		break;
	case EMCOCharacterDirection::Left:
		{
			Direction = -InActor->GetActorRightVector();
		}
		break;
	case EMCOCharacterDirection::Front_Left:
		{
			Direction = (InActor->GetActorForwardVector() + -InActor->GetActorRightVector()).GetSafeNormal();
		}
		break;
	case EMCOCharacterDirection::Front_Right:
		{
			Direction = (InActor->GetActorForwardVector() + InActor->GetActorRightVector()).GetSafeNormal();
		}
		break;
	case EMCOCharacterDirection::Back_Left:
		{
			Direction = (-InActor->GetActorForwardVector() + -InActor->GetActorRightVector()).GetSafeNormal();
		}
		break;
	case EMCOCharacterDirection::Back_Right:
		{
			Direction = (-InActor->GetActorForwardVector() + InActor->GetActorRightVector()).GetSafeNormal();
		}
		break;
	default:
		{
			Direction = InActor->GetActorForwardVector();
		}
		break;
	}

	return Direction;
}