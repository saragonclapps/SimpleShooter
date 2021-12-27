// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGameMode : public ASimpleShooterGameModeBase {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PawnKilled(APawn* PawnKilled) override;

	UFUNCTION(BlueprintCallable, Category = "Kill all enemies")
		void EndGame(bool bIsPlayerWinner);
private:
};
