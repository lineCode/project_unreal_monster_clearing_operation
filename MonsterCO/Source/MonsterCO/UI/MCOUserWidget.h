#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MCOUserWidget.generated.h"

UCLASS()
class MONSTERCO_API UMCOUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
