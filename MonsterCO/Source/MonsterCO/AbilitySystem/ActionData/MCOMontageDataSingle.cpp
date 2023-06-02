#include "AbilitySystem/ActionData/MCOMontageDataSingle.h"
#include "MCOActionDefinition.h"
#include "MCOActionFragment_Montage.h"


UAnimMontage* UMCOMontageDataSingle::GetMontage()
{
	ensure(ActionDefinition);
	const UMCOActionFragment_Montage* MontageFragment = ActionDefinition->GetMontageFragment();
	ensure(MontageFragment);
	return MontageFragment->GetMontage();
}
