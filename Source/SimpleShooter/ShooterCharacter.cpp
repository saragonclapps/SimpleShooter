// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Gun.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

// Sets default values
AShooterCharacter::AShooterCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay() {
	Super::BeginPlay();

	Health = MaxHealth;
	Shield = MaxShield;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

void AShooterCharacter::IsInStuneTime() {
	bIsStune = false;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// UE_LOG(LogTemp, Warning, TEXT("PRE: Damage to apply %f and status shield: %f, health: %f"), DamageToApply, Shield, Health);
	if (!GetWorldTimerManager().IsTimerActive(StuneTimerHandler)) {
		GetWorldTimerManager().SetTimer(StuneTimerHandler, this, &AShooterCharacter::IsInStuneTime, StuneTimer, false);
	}
	else {
		GetWorldTimerManager().ClearTimer(StuneTimerHandler);
		GetWorldTimerManager().SetTimer(StuneTimerHandler, this, &AShooterCharacter::IsInStuneTime, StuneTimer, false);
	}
	bIsStune = true;

	if (Shield > 0) {
		DamageToApply = DamageToApply / 2;
		// TODO: Apply -> FMath::Min(Health, DamageToApply);
		Health -= DamageToApply;
		Shield -= DamageToApply;
		if (Shield < 0) {
			Shield = 0;
		}
	}
	else {
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply;
	}

	if (IsDead()) {
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr) {
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorldTimerManager().SetTimer(DestroyCharacterTimerHandler, this, &AShooterCharacter::DestroyCharacter, DestroyCharacterTimer, false);
	}

	return DamageToApply;
}

void AShooterCharacter::AddHealth(float Amount) {
	if (IsFullHealth()) {
		return;
	}

	Health += Amount;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
}

void AShooterCharacter::AddShield(float Amount) {
	if (IsFullShield()) {
		return;
	}

	Shield += Amount;
	if (Shield > MaxShield) {
		Shield = MaxShield;
	}
}

bool AShooterCharacter::IsFullShield() const {
	return Shield >= MaxShield;
}

float AShooterCharacter::GetShieldPercentage() const {
	return Shield / MaxShield;
}

bool AShooterCharacter::IsFullHealth() const {
	return Health >= MaxHealth;
}

float AShooterCharacter::GetHealthPercentage() const {
	return Health / MaxHealth;
}

bool AShooterCharacter::IsDead() const {
	return Health <= 0;
}

float AShooterCharacter::GetCurrentHealth() const {
	return Health;
}

void AShooterCharacter::SetCurrentHealth(float value){
	Health = value;
}

AGun* AShooterCharacter::GetCurrentGun() {
	return Gun;
}

void AShooterCharacter::DestroyCharacter() {
	GetWorldTimerManager().ClearTimer(StuneTimerHandler);
	GetWorldTimerManager().ClearTimer(DestroyCharacterTimerHandler);
	Gun->Destroy();
	Destroy();
}

bool AShooterCharacter::IsAmmonFull() const {
	return Gun->GetCurrentBulletsAmmunition() == Gun->GetMaxBulletsAmmunition();
}

float AShooterCharacter::GetCurrentWeaponAmmunitionPercentage() const {
	return Gun->GetCurrentBulletsAmmunition() / Gun->GetMaxBulletsAmmunition();
}

float AShooterCharacter::GetCurrentWeaponAmmunition() const {
	return Gun->GetCurrentBulletsAmmunition();
}

void AShooterCharacter::Shoot() {
	if (Gun == nullptr) return;
	Gun->PullTrigger();
}

void AShooterCharacter::MoveForward(float AxisValue) {
	FVector MovementForward = GetActorForwardVector() * AxisValue;
	AddMovementInput(CurrentMovementCalc(MovementForward));
}

void AShooterCharacter::MoveRight(float AxisValue) {
	FVector MovementRight = GetActorRightVector() * AxisValue;
	AddMovementInput(CurrentMovementCalc(MovementRight));
}

void AShooterCharacter::LookUpRate(float AxisValue) {
	float LookUpRate = AxisValue * RotationRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(CurrentMovementCalc(LookUpRate));
}

void AShooterCharacter::LookRightRate(float AxisValue) {
	float LookRightRate = AxisValue * RotationRate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(CurrentMovementCalc(LookRightRate));
}

float AShooterCharacter::CurrentMovementCalc(float RateValue) {
	return bIsStune ? RateValue / 4 : RateValue;
}

FVector AShooterCharacter::CurrentMovementCalc(FVector RateValue) {
	return bIsStune ? RateValue / 4 : RateValue;
}
