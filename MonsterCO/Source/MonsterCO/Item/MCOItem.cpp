#include "Item/MCOItem.h"
#include "MCOItemData_Potion.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/MCOCharacterItemInterface.h"
#include "Physics/MCOPhysics.h"


AMCOItem::AMCOItem()
{
	GETASSET(PickupMontage, UAnimMontage, TEXT("/Game/Items/Egg/Montages/Realistic_EggCracking_Montage.Realistic_EggCracking_Montage"));
	
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("NAME_TriggerBox"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NAME_SkeletalMesh"));
	
	SetRootComponent(Trigger);
	SkeletalMesh->SetupAttachment(Trigger);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Game/Items/Egg/ABP_Item_Egg.ABP_Item_Egg_C"));
	if (true == AnimRef.Succeeded())
	{
		SkeletalMesh->SetAnimInstanceClass(AnimRef.Class);
	}
	
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -45.0f));
	
	Trigger->SetCollisionProfileName(PROFILE_TRIGGER);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	Trigger->SetBoxExtent(FVector(45.0f, 45.0f, 45.0f));
	Trigger->SetHiddenInGame(false);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void AMCOItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (nullptr != Data)
	{
		// Set skeletal mesh 
		if (Data->SkeletalMesh.IsPending())
		{
			Data->SkeletalMesh.LoadSynchronous();
		}
		SkeletalMesh->SetSkeletalMesh(Data->SkeletalMesh.Get());
	
		// Set material
		if (Data->Material.IsPending())
		{
			Data->Material.LoadSynchronous();
		}
		SkeletalMesh->SetMaterial(0, Data->Material.Get());
	}	
	
	InitializeItem(GetActorLocation());
}

void AMCOItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCOItem::SetData(UMCOItemData* InData)
{
	Data = InData;
}

void AMCOItem::InitializeItem(const FVector& InWorldLocation)
{
	FVector RelocationByHeight = InWorldLocation;
	RelocationByHeight.Z += GetItemHalfHeight();
	SetActorLocation(RelocationByHeight);
	
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

float AMCOItem::GetItemHalfHeight() const
{
	return 45.0f;
}

void AMCOItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	SetActorEnableCollision(false);

	IMCOCharacterItemInterface* OverlappingPawn = Cast<IMCOCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Data);
	}
	
	ISTRUE(SkeletalMesh);
	ISTRUE(SkeletalMesh->GetAnimInstance());
	SkeletalMesh->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &ThisClass::OnPickupFinished);
	SkeletalMesh->GetAnimInstance()->Montage_Play(PickupMontage);
}

void AMCOItem::OnPickupFinished(UAnimMontage* Montage, bool bInterrupted)
{
	ISTRUE(Montage == PickupMontage);
	
	if (true == OnItemDestroyed.IsBound())
	{
		OnItemDestroyed.Broadcast();
	}
	
	SetActorHiddenInGame(true);
	//Destroy();
}
