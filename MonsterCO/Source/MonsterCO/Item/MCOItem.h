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
	
public:	
	AMCOItem();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	void SetData(UMCOItemData* InData);
	void InitializeItem(const FVector& InWorldLocation);
	float GetItemHalfHeight() const;
	
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
