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

// Sets default values
AShooterCharacter::AShooterCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay() {
  Super::BeginPlay();

  Health = MaxHealth;

  Gun = GetWorld()->SpawnActor<AGun>(GunClass);
  GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
  Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
  Gun->SetOwner(this);
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

  // Apply damage without going down more than 0 and override the value to DamageToApply
  DamageToApply = FMath::Min(Health, DamageToApply);
  Health -= DamageToApply;
   UE_LOG(LogTemp, Warning, TEXT("The character life is %f and apply damage %f"), Health, DamageToApply);

  if (IsDead()) {
    ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

    if (GameMode != nullptr) {
      GameMode->PawnKilled(this);
    }

    DetachFromControllerPendingDestroy();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }

  return DamageToApply;
}

bool AShooterCharacter::IsDead() const {
  return Health == 0;
}

float AShooterCharacter::GetHealthPercentage() const {
    return Health / MaxHealth;
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
  AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue) {
  AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue) {
  AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue) {
  AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
