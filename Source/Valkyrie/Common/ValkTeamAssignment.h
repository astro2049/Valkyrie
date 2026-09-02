#pragma once

#include "CoreMinimal.h"

class AGameStateBase;
class AValkPlayerState;

namespace ValkTeamAssignment
{
	void AssignTeam(
		const AGameStateBase& aGameState,
		AValkPlayerState& aPlayerState,
		int32 aTeamCount
	);
}
