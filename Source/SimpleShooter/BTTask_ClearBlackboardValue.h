// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "BTTask_ClearBlackboardValue.generated.h"

UCLASS()
class SIMPLESHOOTER_API UBTTask_ClearBlackboardValue : public UBTTask_BlackboardBase {
  GENERATED_BODY()

 public:
  // Sets default values for this node properties
  UBTTask_ClearBlackboardValue();

 protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
