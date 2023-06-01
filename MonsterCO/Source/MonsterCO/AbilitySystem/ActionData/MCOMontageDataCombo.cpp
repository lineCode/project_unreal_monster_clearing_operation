#include "MCOMontageDataCombo.h"
#include "MCOMontageDataUnit.h"

UMCOMontageDataUnit* UMCOMontageDataCombo::GetMontageData(const uint8 InComboIdx)
{
	ensure(ComboMontageData.IsValidIndex(InComboIdx));
	return ComboMontageData[InComboIdx];
}

UAnimMontage* UMCOMontageDataCombo::GetMontage(const uint8 InComboIdx)
{
	ensure(ComboMontageData.IsValidIndex(InComboIdx));
	return ComboMontageData[InComboIdx]->GetMontage();
}

uint8 UMCOMontageDataCombo::GetMaxCombo() const
{
	return ComboMontageData.Num();
}
