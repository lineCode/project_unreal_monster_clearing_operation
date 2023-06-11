#include "Sound/MCOSoundData.h"

USoundCue* UMCOSoundData::GetSoundCue(const EMCOGameState& InState)
{
	ISTRUE_N(true == GameStateSound.Contains(InState));

	return GameStateSound[InState];
}
