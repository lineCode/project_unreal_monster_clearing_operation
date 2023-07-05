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
	bIsActive = false;
	bDestroyOnDeactive = false;
	LifeSpan = 0.0f;
	
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

	bIsActive = true;
	bIsHit = false;
	
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlap);

	StartLifeSpanTimer();
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
	LifeSpan = InLifeSpan;
}

void AMCOProjectile::OnRespawned()
{
	ISTRUE(false == bIsActive);
	
	bIsActive = true;
	
	NiagaraComponent->Activate();
	
	SetActorHiddenInGame(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
	ProjectileMovement->Activate();

	StartLifeSpanTimer();
}

void AMCOProjectile::DestroyActor()
{
	if (false == bIsActive)
	{
		Destroy();
	}
	else
	{
		bDestroyOnDeactive = true;
	}
}

void AMCOProjectile::OnBackToPool()
{
	ISTRUE(true == bIsActive);

	if (true == bDestroyOnDeactive)
	{
		Destroy();
		return;
	}
	
	bIsActive = false;
	LifeSpanTimer.Invalidate();
	
	NiagaraComponent->Deactivate();
	
	SetActorHiddenInGame(true);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement->Deactivate();
	
	ProjectileBackToPoolDelegate.Broadcast(this);
}

void AMCOProjectile::StartLifeSpanTimer()
{
	ISTRUE(true == bIsActive);
	
	LifeSpanTimer.Invalidate();
	
	if (0.0f >= LifeSpan)
	{
		OnBackToPool();
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		LifeSpanTimer,
		this,
		&ThisClass::OnBackToPool,
		LifeSpan,
		false
	);
}

void AMCOProjectile::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	ISTRUE(nullptr != OwnerCharacter);
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	ISTRUE(nullptr != OtherCharacter);
	
	ISTRUE(OwnerCharacter != OtherActor);
	ISTRUE(OwnerCharacter->GetClass() != OtherActor->GetClass());
	
	//MCOLOG_C(MCOCollision, TEXT("+++Projectile OnCollisionBeginOverlap : %s"), *SweepResult.ImpactPoint.ToString());
	
	if (true == CollisionBeginOverlapDelegate.IsBound())
	{
		CollisionBeginOverlapDelegate.Broadcast(OwnerCharacter, OtherCharacter, SweepResult);
	}

	OnBackToPool();
	
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

