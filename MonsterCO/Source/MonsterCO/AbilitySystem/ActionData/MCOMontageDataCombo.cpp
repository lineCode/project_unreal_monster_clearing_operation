#include "MCOMontageDataCombo.h"
#include "MCOActionFragment_Montage.h"

const UMCOActionFragment_Montage* UMCOMontageDataCombo::GetMontageFragment(const uint8 InComboIdx)
{
	ensure(MontageFragments.IsValidIndex(InComboIdx));
	return MontageFragments[InComboIdx];
}

UAnimMontage* UMCOMontageDataCombo::GetMontage(const uint8 InComboIdx)
{
	ensure(MontageFragments.IsValidIndex(InComboIdx));
	return MontageFragments[InComboIdx]->GetMontage();;
}

uint8 UMCOMontageDataCombo::GetMaxCombo() const
{
	return MontageFragments.Num();
}
