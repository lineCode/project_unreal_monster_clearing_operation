#pragma once

#include "MonsterCO.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCOSkillWidgetData.generated.h"

USTRUCT(BlueprintType)
struct FSkillUIData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> SkillImage = nullptr;

	UPROPERTY()
	uint8 SlotLocation = 0;
	
	UPROPERTY()
	FText KeyText = FText();
};




UCLASS()
class MONSTERCO_API UMCOSkillWidgetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void SetData(const FGameplayTag& InTag, const uint8 InSlotLocation, const FText& InKeyText);
	UTexture2D* GetTexture(const FGameplayTag& InTag);
	int8 GetIndex(const FGameplayTag& InTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget)
	TMap<FGameplayTag, FSkillUIData> SkillUIDatas;
};
