// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_OnSearchingAndWaiting.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_OnSearchingAndWaiting : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_OnSearchingAndWaiting();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
