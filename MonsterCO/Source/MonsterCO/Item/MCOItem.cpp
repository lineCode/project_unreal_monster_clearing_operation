#include "Item/MCOItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/MCOPhysics.h"

AMCOItem::AMCOItem()
{
	GETASSET(PickupMontage, UAnimMontage, TEXT("/Game/Items/Egg/Montages/Realistic_EggCracking_Montage.Realistic_EggCracking_Montage"));
	
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("NAME_TriggerBox"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NAME_SkeletalMesh"));
	
	SetRootComponent(Trigger);
	SkeletalMesh->SetupAttachment(Trigger);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Game/Assets/UnkaDragon/Meshes/Eggs/Realistic/Realistic_Egg.Realistic_Egg"));
	if (true == SkeletalMeshRef.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Game/Items/Egg/ABP_Item_Egg.ABP_Item_Egg_C"));
	if (true == AnimRef.Succeeded())
	{
		SkeletalMesh->SetAnimInstanceClass(AnimRef.Class);
	}
	
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -45.0f));
	
	Trigger->SetCollisionProfileName(PROFILE_ABTRIGGER);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	Trigger->SetBoxExtent(FVector(45.0f, 45.0f, 45.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void AMCOItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCOItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	MCOPRINT(TEXT("OVERLAP ITEM EGG"));
	
	SetActorEnableCollision(false);

	ISTRUE(SkeletalMesh);
	ISTRUE(SkeletalMesh->GetAnimInstance());
	SkeletalMesh->GetAnimInstance()->Montage_Play(PickupMontage);
	SkeletalMesh->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::OnPickupFinished);
}

void AMCOItem::OnPickupFinished(UAnimMontage* Montage, bool bInterrupted)
{
	ISTRUE(Montage == PickupMontage);
	
	Destroy();
}
