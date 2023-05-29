#pragma once

#include "MonsterCO.h"
#include "Character/MCOAttachment.h"
#include "Interface/MCOAttackedInterface.h"
#include "MCOMonsterAttachment.generated.h"

UCLASS()
class MONSTERCO_API AMCOMonsterAttachment : public AMCOAttachment, public IMCOAttackedInterface
{
	GENERATED_BODY()
	
public:	
	AMCOMonsterAttachment();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	void TurnOnCollision(const FName& InName);

	UFUNCTION()
	void TurnOffCollision(const FName& InName);

public:
	virtual ACharacter* GetAttackedCharacter() override;
	
};
