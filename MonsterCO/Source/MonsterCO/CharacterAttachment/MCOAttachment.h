#pragma once

#include "MonsterCO.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "MCOAttachment.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAttachmentBeginOverlapDelegate,
												class ACharacter*, InAttacker,
												class AActor*, InAttackCauser,
												class ACharacter*, InOtherCharacter,
												const FHitResult&, SweepResult);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlapDelegate,
												class ACharacter*, InAttacker,
												class AActor*, InAttackCauser,
												class ACharacter*, InOtherCharacter);



UCLASS()
class MONSTERCO_API AMCOAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCOAttachment();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void TurnOnAllCollision();
	
	UFUNCTION()
	void TurnOffAllCollision();
	
protected:
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnAttachmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void AttachActorToSocket(const FName& InSocketName);

	UFUNCTION()
	virtual void AttachCollisionToSocket(UShapeComponent* InComponent, const FName& InSocketName);
	
public:
	UPROPERTY()
	FAttachmentBeginOverlapDelegate OnAttachmentBeginOverlapDelegate;
	
	UPROPERTY()
	FAttachmentEndOverlapDelegate OnAttachmentEndOverlapDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "MCO|Character")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY()
	TArray<TObjectPtr<UShapeComponent>> ShapeComponents;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, TObjectPtr<UShapeComponent>> ShapeComponentsMap;
};
