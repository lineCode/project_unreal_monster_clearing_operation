#include "Item/MCOItem.h"
#include "MCOItemData_Potion.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/MCOCharacterItemInterface.h"
#include "Physics/MCOPhysics.h"

AMCOItem::AMCOItem()
{
	GETASSET(Data, UMCOItemData_Potion, TEXT("/Game/Data/Item/DA_Item_Potion_Small.DA_Item_Potion_Small"));
	GETASSET(PickupMontage, UAnimMontage, TEXT("/Game/Items/Egg/Montages/Realistic_EggCracking_Montage.Realistic_EggCracking_Montage"));
	
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("NAME_TriggerBox"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NAME_SkeletalMesh"));
	
	SetRootComponent(Trigger);
	SkeletalMesh->SetupAttachment(Trigger);
	SkeletalMesh->SetSkeletalMesh(Data->SkeletalMesh);

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

	IMCOCharacterItemInterface* OverlappingPawn = Cast<IMCOCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Data);
	}
	
	ISTRUE(SkeletalMesh);
	ISTRUE(SkeletalMesh->GetAnimInstance());
	SkeletalMesh->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::OnPickupFinished);
	SkeletalMesh->GetAnimInstance()->Montage_Play(PickupMontage);
}

void AMCOItem::OnPickupFinished(UAnimMontage* Montage, bool bInterrupted)
{
	ISTRUE(Montage == PickupMontage);
	
	Destroy();
}
