#include "MCOAttachment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Interface/MCOAttackedInterface.h"
#include "Physics/MCOPhysics.h"


AMCOAttachment::AMCOAttachment()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("NAME_Scene"));
	SetRootComponent(Scene);

	bAttachToSocket = false;
}

void AMCOAttachment::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	AttachToActor(OwnerCharacter, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	
	TArray<UShapeComponent*> ShapeComponents;
	GetComponents<UShapeComponent>(ShapeComponents);
	
	for (UShapeComponent* Shape : ShapeComponents)
	{
		Shape->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttachmentBeginOverlap);
		Shape->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttachmentEndOverlap);

		// Set this in BP
		// Shape->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		// Shape->SetHiddenInGame(false);

		ShapeComponentsMap.Emplace(*Shape->GetName(), Shape);

		if (true == bAttachToSocket)
		{
			AttachCollisionToSocket(Shape, *Shape->GetName());
		}
	}

	bIsAttacking = false;
}

void AMCOAttachment::TurnOnAllCollision() const
{
	for (auto It = ShapeComponentsMap.CreateConstIterator(); It; ++It)
	{
		It.Value()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMCOAttachment::TurnOffAllCollision() const
{
	for (auto It = ShapeComponentsMap.CreateConstIterator(); It; ++It)
	{
		It.Value()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMCOAttachment::TurnOnAttackMode(const FName& InName)
{
	ISTRUE(true == ShapeComponentsMap.Contains(InName));
	
	ShapeComponentsMap[InName]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMCOAttachment::TurnOffAttackMode(const FName& InName)
{
	ISTRUE(true == ShapeComponentsMap.Contains(InName));
	
	ShapeComponentsMap[InName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMCOAttachment::OnBeginCollision(const FCollisionBeginOverlapDelegate& InBeginDelegate,
	const FCollisionEndOverlapDelegate& InEndDelegate, const FName& InSocketName)
{
	bIsAttacking = true;
	
	OnCollisionBeginOverlapDelegate = InBeginDelegate;
	OnCollisionEndOverlapDelegate = InEndDelegate;

	TurnOnAttackMode(InSocketName);
}

void AMCOAttachment::OnEndCollision(const FName& InSocketName)
{
	bIsAttacking = false;
	
	TurnOffAttackMode(InSocketName);
	
	OnCollisionBeginOverlapDelegate.Clear();
	OnCollisionEndOverlapDelegate.Clear();
}

void AMCOAttachment::OnAttachmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ISTRUE(true == bIsAttacking);
	
	MCOLOG_C(MCOCollision, TEXT("++++ OnAttachmentBeginOverlap : %s -> %s (%s)"),
		*OverlappedComponent->GetName(), *OtherComp->GetName(), *SweepResult.ImpactPoint.ToString());
	
	ISTRUE(true == OnCollisionBeginOverlapDelegate.IsBound());
	ISTRUE(OwnerCharacter != OtherActor);
	ISTRUE(OwnerCharacter->GetClass() != OtherActor->GetClass());
	
	const FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Attack),
		false,
		this
	);
	
	TArray<FHitResult> OutHitResults;
	const bool HitDetected = GetWorld()->LineTraceMultiByChannel(
		OutHitResults,
		OverlappedComponent->GetComponentLocation(),
		OtherActor->GetActorLocation(),
		CHANNEL_ACTION_TRACE,
		Params
	);
	
	ISTRUE(true == HitDetected);

	for (const FHitResult& Result : OutHitResults)
	{
		IMCOAttackedInterface* AttackedInterface = Cast<IMCOAttackedInterface>(Result.GetActor());
		if (nullptr == AttackedInterface)
		{
			continue;
		}
		ACharacter* AttackedCharacter = AttackedInterface->GetAttackedCharacter();
		if (nullptr == AttackedCharacter)
		{
			continue;
		}
		if (OwnerCharacter == AttackedCharacter)
		{
			continue;
		}
				
#if ENABLE_DRAW_DEBUG
		
		DrawDebugPoint(
			GetWorld(),
			Result.ImpactPoint,
			10.0f,
			FColor::Blue,
			false,
			3.0f
		);

		DrawDebugLine(
			GetWorld(),
			OverlappedComponent->GetComponentLocation(),
			OtherActor->GetActorLocation(),
			FColor::Blue,
			false,
			3.0f
		);
	
#endif			
	
		OnCollisionBeginOverlapDelegate.Broadcast(OwnerCharacter, AttackedCharacter, Result);
	}
}

void AMCOAttachment::OnAttachmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ISTRUE(true == bIsAttacking);
	
	ISTRUE(OwnerCharacter != OtherActor);
	ISTRUE(OwnerCharacter->GetClass() != OtherActor->GetClass());
	ISTRUE(true == OnCollisionEndOverlapDelegate.IsBound());
	
	OnCollisionEndOverlapDelegate.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

// Attach weapon to character
void AMCOAttachment::AttachActorToSocket(const FName& InSocketName)
{
	ISTRUE(nullptr != OwnerCharacter);
	ISTRUE(nullptr != OwnerCharacter->GetMesh());
	ISTRUE(true == OwnerCharacter->GetMesh()->DoesSocketExist(InSocketName));
	
	AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

// Attach collision to character 
void AMCOAttachment::AttachCollisionToSocket(UShapeComponent* InComponent, const FName& InSocketName)
{
	ISTRUE(nullptr != InComponent);
	ISTRUE(nullptr != OwnerCharacter);
	ISTRUE(nullptr != OwnerCharacter->GetMesh());
	ISTRUE(true == OwnerCharacter->GetMesh()->DoesSocketExist(InSocketName));
	
	InComponent->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);

	// MCOPRINT(FString::Printf(TEXT("Attach Weapon To Socket: %s"), *InSocketName.ToString()));
}
