#pragma once

#include "MonsterCO.h"
#include "Blueprint/UserWidget.h"
#include "MCOAttributeWidget.generated.h"

class UTextBlock;


UCLASS()
class MONSTERCO_API UMCOAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void UpdateHealth(float InValue);
	
	UFUNCTION()
	void UpdateMaxHealth(float InValue);
	
	UFUNCTION()
	void UpdateStamina(float InValue);
	
	UFUNCTION()
	void UpdateMaxStamina(float InValue);
	
	UFUNCTION()
	void UpdateStiffness(float InValue);
	
	UFUNCTION()
	void UpdateMaxStiffness(float InValue);
	
protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> HealthText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxHealthText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> StaminaText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxStaminaText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> StiffnessText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxStiffnessText;
};
