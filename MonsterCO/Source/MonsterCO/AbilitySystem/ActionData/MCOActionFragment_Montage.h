#pragma once

#include "CoreMinimal.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Montage.generated.h"



UCLASS()
class MONSTERCO_API UMCOActionFragment_Montage : public UMCOActionFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UMCOActionDefinition> ActionDefinition;
	
public:
	UAnimMontage* GetMontage() const;
};
