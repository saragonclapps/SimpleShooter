// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ExitSearchingAndWaiting.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "ShooterAIController.h"

UBTTask_ExitSearchingAndWaiting::UBTTask_ExitSearchingAndWaiting() {
	NodeName = "Exit Searching And Waiting";
}

EBTNodeResult::Type UBTTask_ExitSearchingAndWaiting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) {
		return EBTNodeResult::Failed;
	}

	AShooterAIController* Character = Cast<AShooterAIController>(OwnerComp.GetAIOwner());
	if (Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	Character->ExitSearchingAndWaiting();

	return EBTNodeResult::Succeeded;
}
