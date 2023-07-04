#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ExecuteCue.generated.h"

UCLASS()
class MONSTERCO_API UAnimNotify_ExecuteCue : public UAnimNotify
{
	GENERATED_BODY()

protected:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
};
