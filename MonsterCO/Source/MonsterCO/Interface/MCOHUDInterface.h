#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "MCOHUDInterface.generated.h"

class UMCOHUDWidget;
class UMCOStaminaWidget;
class IMCOCharacterInterface;

UINTERFACE(MinimalAPI)
class UMCOHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class MONSTERCO_API IMCOHUDInterface
{
	GENERATED_BODY()

// --- HUD
public:
	virtual void InitializeHUD(UMCOHUDWidget* InHUDWidget) = 0;
	virtual void ShowMonsterInfo(IMCOCharacterInterface* InCharacter) = 0;

// ---- Cooldown
public:
	// virtual void StartCooldownWidget(const FGameplayTag& InTag, const float& InCooldownTime) const = 0;

// --- Stamina
public:
	virtual void SetupStaminaWidget(UMCOStaminaWidget* InStaminaWidget) = 0;
};
