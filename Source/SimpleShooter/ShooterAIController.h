// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "ShooterAIController.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController {
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PatrolSearching(float DeltaTime);

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	virtual void OnSearchingAndWaiting();

	virtual void ExitSearchingAndWaiting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool IsSearchingAndWaiting;

	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
		float SearchLastTargetPositionCountDown;

	UPROPERTY()
		FVector LastKnownPlayerLocation;

	UPROPERTY()
		float ScoutTime;

	UPROPERTY(EditDefaultsOnly)
		float MaxScoutAngle = 90;

	UPROPERTY(EditDefaultsOnly)
		float ScoutSpeed = 0.3;

	UPROPERTY()
		AActor* Target;

	UPROPERTY()
		AActor* PawnControlled;
};
