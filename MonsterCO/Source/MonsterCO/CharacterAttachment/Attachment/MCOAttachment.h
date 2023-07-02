#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Interface/MCOCharacterInterface.h"
#include "MCOAttachment.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllCollisionChangedDelegate, const FName&, InName);



UCLASS()
class MONSTERCO_API AMCOAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOAttachment();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MCO|Character")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Scene;

// --- Equip
public:
	bool GetIsEquipped() const { return bIsEquipped; }

protected:
	UPROPERTY()
	uint8 bIsEquipped:1;

// --- Collision
public:
	UFUNCTION()
	void TurnOnAllCollision() const;

	UFUNCTION()
	void TurnOffAllCollision() const;
	
	UFUNCTION()
	virtual void TurnOnAttackMode(const FName& InName);

	UFUNCTION()
	virtual void TurnOffAttackMode(const FName& InName);


protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, TObjectPtr<UShapeComponent>> ShapeComponentsMap;

// --- Overlap
public:
	void OnBeginCollision(const FCollisionBeginOverlapDelegate& InBeginDelegate, const FCollisionEndOverlapDelegate& InEndDelegate, const FName& InSocketName);
	void OnEndCollision(const FName& InSocketName);
	
protected:
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttachmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void AttachActorToSocket(const FName& InSocketName);

	UFUNCTION()
	virtual void AttachCollisionToSocket(UShapeComponent* InComponent, const FName& InSocketName);

protected:
	UPROPERTY()
	FCollisionBeginOverlapDelegate OnCollisionBeginOverlapDelegate;
	
	UPROPERTY()
	FCollisionEndOverlapDelegate OnCollisionEndOverlapDelegate;

protected:
	UPROPERTY()
	uint8 bAttachToSocket:1;

	bool bIsAttacking;
};
