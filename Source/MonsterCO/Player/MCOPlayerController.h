#pragma once

#include "MonsterCO.h"
#include "GameFramework/PlayerController.h"
#include "MCOPlayerController.generated.h"

class UMCOAbilitySystemComponent;
class UMCOHUDWidget;


UCLASS()
class MONSTERCO_API AMCOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCOPlayerController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

// --- Ability
public:
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;
	
// --- Input
protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
// --- Widget
protected:
	UPROPERTY()
	TSubclassOf<UMCOHUDWidget> HUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UMCOHUDWidget> HUDWidget;
};
