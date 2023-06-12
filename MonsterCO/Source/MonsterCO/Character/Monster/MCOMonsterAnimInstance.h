#pragma once

#include "MonsterCO.h"
#include "Character/MCOCharacterAnimInstance.h"
#include "Interface/MCOMonsterAIInterface.h"
#include "MCOMonsterAnimInstance.generated.h"


UCLASS()
class MONSTERCO_API UMCOMonsterAnimInstance : public UMCOCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void SetPawnDirection(const FVector& InVelocity, const FRotator& InActorRotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<IMCOMonsterAIInterface> MonsterAIInterface;
};
