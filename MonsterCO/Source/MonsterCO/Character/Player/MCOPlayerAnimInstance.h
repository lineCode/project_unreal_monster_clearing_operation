#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacterAnimInstance.h"
#include "Interface/MCOPlayerInterface.h"
#include "MCOPlayerAnimInstance.generated.h"


UCLASS()
class MONSTERCO_API UMCOPlayerAnimInstance : public UMCOCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void SetPawnSpeed(const FVector& InVelocity, const float& InDeltaSeconds) override;
	virtual void SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	EMCOPlayerMode ModeType; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsDodging:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint32 bIsEquipped:1;
};
