// Fill out your copyright notice in the Description page of Project Settings.

#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "Engine/Engine.h"
#include "ShooterCharacter.h"
#include "GameFramework/PlayerController.h"

#define OUT

void AKillEmAllGameMode::BeginPlay() {
	Super::BeginPlay();
}

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) {
	Super::PawnKilled(PawnKilled);
	UE_LOG(LogTemp, Error, TEXT("Pawn kill"));

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr) {
		EndGame(false);
	}

	//for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())) {
	//	if (!Controller->IsDead()) {
	//		return;
	//	}
	//}

	//EndGame(true);
}

void AKillEmAllGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) {
	for (AController* Controller : TActorRange<AController>(GetWorld())) {
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}