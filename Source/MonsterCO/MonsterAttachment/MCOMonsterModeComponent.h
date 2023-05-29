#pragma once

#include "MonsterCO.h"
#include "Components/ActorComponent.h"
#include "MCOMonsterModeComponent.generated.h"

class AMCOMonsterAttachment;


UCLASS()
class MONSTERCO_API UMCOMonsterModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMCOMonsterModeComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE AMCOMonsterAttachment* GetAttachment() { return Attachment; }
	void SpawnAttachment(ACharacter* InOwner);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMCOMonsterAttachment> Attachment;
	
	UPROPERTY()
	TSubclassOf<AMCOMonsterAttachment> AttachmentClass;
};
