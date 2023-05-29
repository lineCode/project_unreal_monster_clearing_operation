#pragma once

#include "MonsterCO.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MCOInputConfig.generated.h"


class UInputAction;
class UInputMappingContext;


USTRUCT(BlueprintType)
struct FMCOInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	FGameplayTag InputTag;
};

UCLASS()
class MONSTERCO_API UMCOInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TArray<FMCOInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TArray<FMCOInputAction> AbilityInputActions;
};
