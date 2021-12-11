// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Game Status")
	void OnPauseEvent();

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HudScreenClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> PauseScreenClass;

	UPROPERTY(EditAnywhere)
		float RestartDelay = 5;

	FTimerHandle RestartTimer;
	bool HasBeenExecuted;

	UUserWidget* Hud;

	UUserWidget* PauseMenu;

	bool bPause;
};
