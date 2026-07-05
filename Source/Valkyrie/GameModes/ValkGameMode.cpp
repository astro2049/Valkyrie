// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

AValkGameMode::AValkGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerStateClass = AValkPlayerState::StaticClass();
}

void AValkGameMode::PostLogin(APlayerController* const aNewPlayer)
{
	if (AValkPlayerState* const playerState = aNewPlayer ? aNewPlayer->GetPlayerState<AValkPlayerState>() : nullptr;
		playerState && playerState->GetTeamId() == EValkTeamId::None) {
		playerState->SetTeamId(EValkTeamId::TeamA);
	}

	Super::PostLogin(aNewPlayer);
}

AActor* AValkGameMode::ChoosePlayerStart_Implementation(AController* const aPlayer)
{
	const AValkPlayerState* const playerState = aPlayer ? aPlayer->GetPlayerState<AValkPlayerState>() : nullptr;
	if (!playerState) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	FName playerStartTag;
	switch (playerState->GetTeamId()) {
	case EValkTeamId::TeamA:
		playerStartTag = FName(TEXT("TeamA"));
		break;
	case EValkTeamId::TeamB:
		playerStartTag = FName(TEXT("TeamB"));
		break;
	default:
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}
	TArray<AActor*> playerStartActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStartActors);
	TArray<APlayerStart*> teamPlayerStarts;
	for (AActor* const playerStartActor : playerStartActors) {
		if (APlayerStart* const playerStart = Cast<APlayerStart>(playerStartActor);
			playerStart && playerStart->PlayerStartTag == playerStartTag) {
			teamPlayerStarts.Add(playerStart);
		}
	}

	if (teamPlayerStarts.IsEmpty()) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	teamPlayerStarts.Sort([](const APlayerStart& aLeft, const APlayerStart& aRight) {
		return aLeft.GetName() < aRight.GetName();
	});
	int32 playerIndex = 0;
	for (FConstPlayerControllerIterator iterator = GetWorld()->GetPlayerControllerIterator(); iterator; ++iterator) {
		const APlayerController* const playerController = iterator->Get();
		const AValkPlayerState* const currentPlayerState = playerController
			? playerController->GetPlayerState<AValkPlayerState>()
			: nullptr;
		if (currentPlayerState && currentPlayerState->GetTeamId() == playerState->GetTeamId()) {
			if (playerController == aPlayer) {
				break;
			}
			++playerIndex;
		}
	}

	return teamPlayerStarts[playerIndex % teamPlayerStarts.Num()];
}

void AValkGameMode::RestartPlayer(AController* const aNewPlayer)
{
	Super::RestartPlayer(aNewPlayer);
	BindPlayerDeath(aNewPlayer);
}

void AValkGameMode::OnPlayerDeath(AController* const, AController* const aVictimController)
{
	if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(aVictimController)) {
		playerController->Client_OnPlayerDeath();
	}
}

void AValkGameMode::ScheduleReturnToMainMenu()
{
	if (GetWorldTimerManager().IsTimerActive(myReturnTimerHandle)) {
		return;
	}

	GetWorldTimerManager().SetTimer(
		myReturnTimerHandle,
		this,
		&AValkGameMode::ReturnPlayersToMainMenu,
		myPostMatchDelay,
		false
	);
}

void AValkGameMode::BindPlayerDeath(AController* const aController)
{
	APawn* const pawn = aController ? aController->GetPawn() : nullptr;
	if (UHealthComponent* const healthComponent = pawn ? pawn->FindComponentByClass<UHealthComponent>() : nullptr) {
		healthComponent->myOnHealthDeath.AddUObject(
			this,
			&AValkGameMode::OnPlayerDeath,
			aController
		);
	}
}

void AValkGameMode::ReturnPlayersToMainMenu() const
{
	if (UWorld* const world = GetWorld()) {
		world->ServerTravel(TEXT("/Game/MainMenu/MainMenu"));
	}
}
