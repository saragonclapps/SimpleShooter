// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();
	void PullTrigger();
	float GetMaxBulletsAmmunition();
	float GetCurrentBulletsAmmunition();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Shooter Character")
		void AddAmmon(float Amount);
private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnyWhere)
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnyWhere)
		USoundBase* MuzzleSound;
	
	UPROPERTY(EditAnyWhere)
		UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnyWhere)
		USoundBase* ImpactSound;

	UPROPERTY(EditAnyWhere)
		float MaxRange = 10000;

	UPROPERTY(EditAnyWhere)
		float Damage = 10;

	UPROPERTY(EditAnyWhere)
		float MaxBulletsAmmunition = 500;

	UPROPERTY(VisibleAnywhere)
		float CurrentBulletsAmmunition;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;
};
