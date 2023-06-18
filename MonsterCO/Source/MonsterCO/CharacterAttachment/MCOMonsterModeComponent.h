#pragma once

#include "MonsterCO.h"
#include "MCOModeComponent.h"
#include "MCOMonsterModeComponent.generated.h"

class AMCOMonsterAttachment;


UCLASS()
class MONSTERCO_API UMCOMonsterModeComponent : public UMCOModeComponent
{
	GENERATED_BODY()

public:	
	UMCOMonsterModeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SpawnAttachment(ACharacter* InOwner) override;
	
protected:	
	UPROPERTY()
	TSubclassOf<AMCOMonsterAttachment> AttachmentClass;
};
