#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacterAnimInstance.h"
#include "MCOMonsterAnimInstance.generated.h"


UCLASS()
class MONSTERCO_API UMCOMonsterAnimInstance : public UMCOCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void SetPawnSpeed(const FVector& InVelocity, const float& InDeltaSeconds) override;
	virtual void SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsFlying : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float CurrentVerticalSpeed;
};
