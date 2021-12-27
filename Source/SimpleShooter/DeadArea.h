// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "DeadArea.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API ADeadArea : public ATriggerVolume
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	void OnDeadPlayerCharacter(AActor* PlayerActor);
public:
	// constructor sets default values for this actor's properties
	ADeadArea();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
