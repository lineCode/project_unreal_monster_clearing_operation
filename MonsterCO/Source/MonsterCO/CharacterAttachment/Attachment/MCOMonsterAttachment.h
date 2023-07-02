#pragma once

#include "MonsterCO.h"
#include "MCOAttachment.h"
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
	virtual ACharacter* GetAttackedCharacter() override;
	
	virtual void TurnOnAttackMode(const FName& InName) override;
	virtual void TurnOffAttackMode(const FName& InName) override;
};
