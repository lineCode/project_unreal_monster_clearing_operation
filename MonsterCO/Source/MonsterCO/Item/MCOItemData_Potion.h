#pragma once

#include "CoreMinimal.h"
#include "MCOItemData.h"
#include "MCOItemData_Potion.generated.h"

UCLASS()
class MONSTERCO_API UMCOItemData_Potion : public UMCOItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
};
