// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "ToxicArea.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API AToxicArea : public ATriggerVolume
{
	GENERATED_BODY()

protected:
	float BeforeLife;
	float ToDecreseLife = 20.0f;

	virtual void BeginPlay() override;
public:
	// constructor sets default values for this actor's properties
	AToxicArea();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
};
