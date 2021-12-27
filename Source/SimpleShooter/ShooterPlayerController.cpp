// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void AShooterPlayerController::BeginPlay() {
	Super::BeginPlay();

	Hud = CreateWidget(this, HudScreenClass);
	if (Hud != nullptr) {
		Hud->AddToViewport();
	}

	PauseMenu = CreateWidget(this, PauseScreenClass);


	if (PauseMenu == nullptr) {
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("Is not exist the PAUSE SCREEN WIDGET"));
#endif
		return;
	}
}

void AShooterPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &AShooterPlayerController::OnPauseEvent);
}

void AShooterPlayerController::NextLevel() {
	UGameplayStatics::OpenLevel(GetWorld(), NameForNextLevel);
}

void AShooterPlayerController::OnPauseEvent() {
	bPause = !bPause;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, bPause ? TEXT("true") : TEXT("false"));

	if (bPause) {
		//UGameplayStatics::SetGamePaused(GetWorld(), bPause);
		Hud->RemoveFromViewport();
		PauseMenu->AddToViewport();	
		bShowMouseCursor = true;
		SetPause(bPause);
		return;
	}

	Hud->AddToViewport();
	PauseMenu->RemoveFromViewport();
	bShowMouseCursor = false;
	SetPause(bPause);
}

void AShooterPlayerController::SetNextLevelName(FName Name) {
	this->NameForNextLevel = Name;
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) {
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	if (this->HasBeenExecuted) {
		return;
	}

	Hud->RemoveFromViewport();
	this->HasBeenExecuted = true;
	if (bIsWinner) {
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr) {
			WinScreen->AddToViewport();
		}

		//GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
		GetWorldTimerManager().SetTimer(RestartTimer, this, &AShooterPlayerController::NextLevel, RestartDelay);
		return;
	}

	UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
	if (LoseScreen != nullptr) {
		LoseScreen->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
