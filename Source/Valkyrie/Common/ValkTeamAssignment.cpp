#include "ValkTeamAssignment.h"

#include "GameFramework/GameStateBase.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

void ValkTeamAssignment::AssignTeam(
	const AGameStateBase& aGameState,
	AValkPlayerState& aPlayerState,
	const int32 aTeamCount
)
{
	if (aPlayerState.GetTeamId() != EValkTeamId::None) {
		return;
	}

	if (aTeamCount == 1) {
		aPlayerState.SetTeamId(EValkTeamId::TeamA);
		return;
	}

	int32 teamAPlayerCount = 0;
	int32 teamBPlayerCount = 0;
	for (const APlayerState* const playerState : aGameState.PlayerArray) {
		const EValkTeamId teamId = CastChecked<AValkPlayerState>(playerState)->GetTeamId();
		if (teamId == EValkTeamId::TeamA) {
			teamAPlayerCount++;
		} else if (teamId == EValkTeamId::TeamB) {
			teamBPlayerCount++;
		}
	}

	aPlayerState.SetTeamId(teamAPlayerCount <= teamBPlayerCount ? EValkTeamId::TeamA : EValkTeamId::TeamB);
}
