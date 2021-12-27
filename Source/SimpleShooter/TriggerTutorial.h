// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TriggerTutorial.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API ATriggerTutorial : public ATriggerVolume
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> TutorialScreenClass;

	UUserWidget* TutorialScreen;

public:

	// constructor sets default values for this actor's properties
	ATriggerTutorial();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
};
