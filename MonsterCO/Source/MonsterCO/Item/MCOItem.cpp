#include "Item/MCOItem.h"
#include "Engine/AssetManager.h"
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
	SkeletalMesh->SetSkeletalMesh(Data->SkeletalMesh);
	
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
	
	Trigger->SetCollisionProfileName(PROFILE_ABTRIGGER);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	Trigger->SetBoxExtent(FVector(45.0f, 45.0f, 45.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	
}

void AMCOItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(ITEMDATA_NAME, Assets);
	ensure(0 < Assets.Num());

	MCOPRINT(TEXT("%d"), Assets.Num());
	
	const int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	
	const FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (true == AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	Data = Cast<UMCOItemData>(AssetPtr.Get());

	// Set material
	SkeletalMesh->SetMaterial(0, Data->Material);
	
	ensure(nullptr != Data);
}

void AMCOItem::BeginPlay()
{
	Super::BeginPlay();
	
}

float AMCOItem::GetItemHalfHeight() const
{
	return 45.0f / 2.0f;
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
	SkeletalMesh->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::OnPickupFinished);
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
	Destroy();
}
