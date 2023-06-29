#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "MCOItem.generated.h"

class UBoxComponent;
class UMCOItemData;


DECLARE_MULTICAST_DELEGATE(FMCOOnItemDestroyed);



UCLASS()
class MONSTERCO_API AMCOItem : public AActor
{
	GENERATED_BODY()

// --- Set
public:	
	AMCOItem();

public:
	void SetData(UMCOItemData* InData);
	
protected:
	virtual void PostInitializeComponents() override;
	void InitializeItem(const FVector& InWorldLocation);
	virtual void BeginPlay() override;
	
// --- Get
protected:
	float GetItemHalfHeight() const;

// --- Collision
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnPickupFinished(UAnimMontage* Montage, bool bInterrupted);

public:
	FMCOOnItemDestroyed OnItemDestroyed;
	
protected:
	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> PickupMontage;

	UPROPERTY(EditAnywhere, Category = Data)
	TObjectPtr<UMCOItemData> Data;
};
