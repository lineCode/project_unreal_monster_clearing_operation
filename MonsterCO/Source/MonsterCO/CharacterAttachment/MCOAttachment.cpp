#include "MCOAttachment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"


AMCOAttachment::AMCOAttachment()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("NAME_Scene"));
	SetRootComponent(Scene);
}

void AMCOAttachment::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	AttachToActor(OwnerCharacter, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* Shape : ShapeComponents)
	{
		Shape->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttachmentBeginOverlap);
		Shape->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttachmentEndOverlap);

		// Set this in BP
		// Shape->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		// Shape->SetHiddenInGame(false);

		ShapeComponentsMap.Emplace(*Shape->GetName(), Shape);
		AttachCollisionToSocket(Shape, *Shape->GetName());
	}

	// TurnOffAllCollision();
}

void AMCOAttachment::TurnOnAllCollision()
{
	for (UShapeComponent* Shape : ShapeComponents)
	{
		Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AMCOAttachment::TurnOffAllCollision()
{
	for (UShapeComponent* Shape : ShapeComponents)
	{
		// Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMCOAttachment::OnAttachmentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ISTRUE(OwnerCharacter != OtherActor);
	ISTRUE(OwnerCharacter->GetClass() != OtherActor->GetClass());
	ISTRUE(true == OnAttachmentBeginOverlapDelegate.IsBound());
	
	OnAttachmentBeginOverlapDelegate.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor), SweepResult);
}

void AMCOAttachment::OnAttachmentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ISTRUE(OwnerCharacter != OtherActor);
	ISTRUE(OwnerCharacter->GetClass() != OtherActor->GetClass());
	ISTRUE(true == OnAttachmentEndOverlapDelegate.IsBound());

	OnAttachmentEndOverlapDelegate.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

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
