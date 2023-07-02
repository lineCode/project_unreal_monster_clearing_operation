#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ActionData/MCOActionDefinition.h"
#include "MCOActionFragment_Projectile.generated.h"


class AMCOProjectile;



UCLASS()
class MONSTERCO_API UMCOActionFragment_Projectile : public UMCOActionFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile)
	TSubclassOf<AMCOProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float ProjectileLifeSpan = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	float ProjectileSpeed = 600.0f;
};
