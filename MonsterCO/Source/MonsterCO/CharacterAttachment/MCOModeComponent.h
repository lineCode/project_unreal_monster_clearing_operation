#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOModeComponent.generated.h"


class AMCOAttachment;


UCLASS()
class MONSTERCO_API UMCOModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMCOModeComponent();

protected:
	virtual void BeginPlay() override;

// --- Getter
public:
	AMCOAttachment* GetCurrentAttachment() const;
	FAttachmentBeginOverlapDelegate& GetAttachmentBeginOverlapDelegate() const;

// --- Mode (1) 
public:
	int32 GetCurrentMode() const;
	void SetCurrentMode(const int32 InIdx);
	
// --- Spawn (2) 
public:
	virtual void SpawnAttachment(ACharacter* InOwner) {};
	
protected:
	void SpawnAttachmentByClass(ACharacter* InOwner, const TSubclassOf<AMCOAttachment>& InAttachmentClass);
	
// --- Equip (3) 
public:
	bool IsEquipped() const;
	virtual void SetEquipUnequipInstantly(const bool bEquip) {};
	virtual void SwitchEquipUnequip() {};

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AMCOAttachment>> Attachments;

	UPROPERTY()
	int32 CurrentAttachmentIdx;
};
