#pragma once

#include "MonsterCO.h"
#include "Components/ActorComponent.h"
#include "MCOPawnExtensionComponent.generated.h"


class UMCOAbilitySystemComponent;


UCLASS()
class MONSTERCO_API UMCOPawnExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMCOPawnExtensionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	/** Returns the pawn extension component if one exists on the specified actor. */
	static UMCOPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor);
	
	/** Should be called by the owning pawn when the pawn's controller changes. */
	void HandleControllerChanged();
	
	/** Should be called by the owning pawn to become the avatar of the ability system. */
	void InitializeAbilitySystem(UMCOAbilitySystemComponent* InASC, AActor* InOwnerActor);

	/** Should be called by the owning pawn to remove itself as the avatar of the ability system. */
	void UninitializeAbilitySystem();
	
protected:
	UPROPERTY()
	TObjectPtr<UMCOAbilitySystemComponent> AbilitySystemComponent;
};
