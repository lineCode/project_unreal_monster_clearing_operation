#pragma once

#include "MonsterCO.h"
#include "GameFramework/PlayerController.h"
#include "MCOPlayerController.generated.h"

class UMCOAbilitySystemComponent;
class UMCOMainWidget;
class UMCOMainWidgetData;


UCLASS()
class MONSTERCO_API AMCOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCOPlayerController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnGameStateChanged(const EMCOGameState& InState);

	
// --- Ability
public:
	UMCOAbilitySystemComponent* GetMCOAbilitySystemComponent() const;
	
// --- Input
protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
// --- Widget
protected:
	void CreateWidgetAndAddToViewport(TSubclassOf<UMCOMainWidget>& InClass, EMCOWidgetState InState);
	void ChangeWidgetByState(EMCOWidgetState InState);
	void ChangeInputMode(bool bGameMode);

public:
	void OnOptionKeyPressed();
	
protected:
	UFUNCTION()
	void ExitGame();
	
	UFUNCTION()
	void OnWidgetClosedByUser();
	
protected:
	UPROPERTY()
	TObjectPtr<UMCOMainWidgetData> WidgetData;
	
	UPROPERTY()
	TMap<EMCOWidgetState, TObjectPtr<UMCOMainWidget>> Widgets;
	
	EMCOWidgetState PreviousWidgetState;
	EMCOWidgetState CurrentWidgetState;
};
