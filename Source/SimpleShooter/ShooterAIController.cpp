// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay() {
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Target = PlayerPawn;
	PawnControlled = AController::GetPawn();

	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), PawnControlled->GetActorLocation());
	}
}

void AShooterAIController::OnSearchingAndWaiting(){
	IsSearchingAndWaiting = true;
}

void AShooterAIController::ExitSearchingAndWaiting() {
	IsSearchingAndWaiting = false;
}

void AShooterAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (IsSearchingAndWaiting) {
		this->PatrolSearching(DeltaTime);
	}
}

void AShooterAIController::PatrolSearching(float DeltaTime) {
	ScoutTime += DeltaTime;
	FVector LastPositionDirection = (PawnControlled->GetActorLocation() + LastKnownPlayerLocation) / 2;

	float RotationScout = MaxScoutAngle * FMath::Sin(ScoutSpeed * ScoutTime);
	FRotator RstRotation = LastPositionDirection.RotateAngleAxis(RotationScout, FVector(0, 0, 1)).Rotation();
	PawnControlled->SetActorRotation(RstRotation);
}

bool AShooterAIController::IsDead() const {
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ControlledCharacter != nullptr) {
		return ControlledCharacter->IsDead();
	}

	return true;
}
