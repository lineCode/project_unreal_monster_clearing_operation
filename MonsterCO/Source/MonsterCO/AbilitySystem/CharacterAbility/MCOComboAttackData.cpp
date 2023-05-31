#include "MCOComboAttackData.h"
#include "MCOCommonMontageData.h"

UMCOComboAttackData::UMCOComboAttackData()
{
}

float UMCOComboAttackData::GetComboCheckTime(uint8 CurrentCombo, float AttackSpeedRate)
{
	return (MontageData[CurrentCombo - 1].NextComboFrameCount / MontageData[CurrentCombo - 1].Data->FrameRate) / AttackSpeedRate;
}

UMCOCommonMontageData* UMCOComboAttackData::GetMontageData(uint8 CurrentCombo)
{
	ensure(MontageData.IsValidIndex(CurrentCombo - 1));
	return MontageData[CurrentCombo - 1].Data;
}

UAnimMontage* UMCOComboAttackData::GetMontage(uint8 CurrentCombo)
{
	return GetMontageData(CurrentCombo)->MontageToPlay;
}
