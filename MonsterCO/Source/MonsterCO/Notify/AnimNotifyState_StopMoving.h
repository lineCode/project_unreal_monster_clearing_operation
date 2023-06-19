#pragma once

#include "MonsterCO.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_StopMoving.generated.h"

UCLASS()
class MONSTERCO_API UAnimNotifyState_StopMoving : public UAnimNotifyState
{
	GENERATED_BODY()
		
public:
	FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
