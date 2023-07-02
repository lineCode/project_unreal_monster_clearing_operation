#include "Projectile/MCOProjectile.h"
#include "NiagaraComponent.h"
#include "Chaos/AABBTree.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/MCOAttackedInterface.h"
#include "Physics/MCOPhysics.h"


AMCOProjectile::AMCOProjectile()
{
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("NAME_Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName(PROFILE_PROJECTILE);
	Sphere->SetHiddenInGame(false);
	Sphere->SetGenerateOverlapEvents(true);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NAME_Niagara"));
	NiagaraComponent->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("NAME_ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AMCOProjectile::BeginPlay()
{
	Super::BeginPlay();

	bIsHit = false;
	
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlap);
	Sphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionEndOverlap);
}

void AMCOProjectile::Destroyed()
{
	if (false == bIsHit && false == HasAuthority())
	{
		// sound, effect 
	}
	
	Super::Destroyed();
}

void AMCOProjectile::Initialize(const float& InSpeed, const float& InLifeSpan)
{
	ProjectileMovement->InitialSpeed = InSpeed;
	ProjectileMovement->MaxSpeed = InSpeed;
	SetLifeSpan(InLifeSpan);
}

// void AMCOProjectile::SetDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InDamageEffectSpecHandle)
// {
// 	DamageEffectSpecHandle = InDamageEffectSpecHandle;
// }

void AMCOProjectile::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	ISTRUE(nullptr != OwnerCharacter);
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	ISTRUE(nullptr != OtherCharacter);
	
	ISTRUE(OwnerCharacter != OtherActor);
	ISTRUE(OwnerCharacter->GetClass() != OtherActor->GetClass());
	
	MCOLOG_C(MCOCollision, TEXT("+++Projectile OnCollisionBeginOverlap : %s"), *SweepResult.ImpactPoint.ToString());
	
	ISTRUE(true == CollisionBeginOverlapDelegate.IsBound());
	
	CollisionBeginOverlapDelegate.Broadcast(Cast<ACharacter>(GetOwner()), OtherCharacter, SweepResult);

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(0.1f);
	
	// if (true == HasAuthority())
	// {
	// 	SetLifeSpan(0.1f);
	// 	SetActorHiddenInGame(true);
	// }
	// else
	// {
	// 	bIsHit = true;
	// }
}

void AMCOProjectile::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


