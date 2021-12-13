// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;


UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Override method inherits from Actor Class
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();

	FTimerHandle FireRateTimerHandle;
	
	float StuneTimmer = 2.0f;

	bool bIsStune;

	UFUNCTION(BlueprintCallable, Category = "Shooter Character")
		void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Shooter Character")
		void AddShield(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Shooter Character")
		AGun* GetCurrentGun();

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	UFUNCTION(BlueprintPure)
		bool IsFullHealth() const;

	UFUNCTION(BlueprintPure)
		bool IsFullShield() const;

	UFUNCTION(BlueprintPure)
		bool IsAmmonFull() const;

	UFUNCTION(BlueprintPure)
		float GetHealthPercentage() const;

	UFUNCTION(BlueprintPure)
		float GetShieldPercentage() const;

	UFUNCTION(BlueprintPure)
		float GetCurrentWeaponAmmunitionPercentage() const;

	UFUNCTION(BlueprintPure)
		float GetCurrentWeaponAmmunition() const;
private:
	UPROPERTY(EditAnywhere)
		float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass;

	UPROPERTY()
		AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
		float Health;

	UPROPERTY(EditDefaultsOnly)
		float MaxShield = 100;

	UPROPERTY(VisibleAnywhere)
		float Shield;

	// Input movement
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
};
