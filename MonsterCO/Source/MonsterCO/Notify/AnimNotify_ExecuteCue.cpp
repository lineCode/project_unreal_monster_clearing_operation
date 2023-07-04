#include "Notify/AnimNotify_ExecuteCue.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

FString UAnimNotify_ExecuteCue::GetNotifyName_Implementation() const
{
	return Tag.ToString();
}

void UAnimNotify_ExecuteCue::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	ISTRUE(nullptr != MeshComp);
	ISTRUE(nullptr != MeshComp->GetOwner());
	
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	ISTRUE(nullptr != ASCInterface);

	UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
	ISTRUE(nullptr != ASC);

	ASC->ExecuteGameplayCue(Tag);
}

