// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_OnSearchingAndWaiting.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "ShooterAIController.h"

UBTTask_OnSearchingAndWaiting::UBTTask_OnSearchingAndWaiting() {
	NodeName = "On Searching And Waiting";
}

EBTNodeResult::Type UBTTask_OnSearchingAndWaiting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) {
		return EBTNodeResult::Failed;
	}

	AShooterAIController* Character = Cast<AShooterAIController>(OwnerComp.GetAIOwner());
	if (Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	Character->OnSearchingAndWaiting();

	return EBTNodeResult::Succeeded;
}
