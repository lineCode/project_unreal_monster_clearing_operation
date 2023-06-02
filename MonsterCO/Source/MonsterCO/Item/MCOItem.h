#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "MCOItem.generated.h"

class UBoxComponent;


UCLASS()
class MONSTERCO_API AMCOItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOItem();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnPickupFinished(UAnimMontage* Montage, bool bInterrupted);
	
protected:
	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Trigger;

protected:
	UPROPERTY(VisibleAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> PickupMontage;
	
};
