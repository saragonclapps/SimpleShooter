// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "TriggerTutorial.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#define OUT

// Sets default values
AGun::AGun() {
	// Set this actor to call Tick() every frame.  You can turn this off to
	// improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentBulletsAmmunition = MaxBulletsAmmunition;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay() {
	Super::BeginPlay();

	ParamsIgnore.AddIgnoredActor(this);
	ParamsIgnore.AddIgnoredActor(GetOwner());

	//TArray<AActor*> FoundTriggerTutorials;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerTutorial::StaticClass(), OUT FoundTriggerTutorials);

	for (ATriggerTutorial* TriggerTutorial : TActorRange<ATriggerTutorial>(GetWorld())) {
		//ATriggerVolume* TriggerVolume = Cast<ATriggerVolume>(TriggerTutorial->GetComponentByClass(ATriggerVolume::StaticClass()));
		ParamsIgnore.AddIgnoredActor(TriggerTutorial);
	}
}

void AGun::PullTrigger() {
	if (MuzzleFlash == nullptr || MuzzleSound == nullptr || CurrentBulletsAmmunition <= 0) return;
	CurrentBulletsAmmunition -= 1;
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (!bSuccess) return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location, ShotDirection.Rotation());

	AActor* HitActor = Hit.GetActor();
	if (HitActor == nullptr) return;

	FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
	AController *OwnerController = GetOwnerController();
	HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
}

void AGun::AddAmmon(float Amount) {
	if (CurrentBulletsAmmunition == MaxBulletsAmmunition) {
		return;
	}

	CurrentBulletsAmmunition += Amount;
	if (CurrentBulletsAmmunition > MaxBulletsAmmunition) {
		CurrentBulletsAmmunition = MaxBulletsAmmunition;
	}
}

float AGun::GetCurrentBulletsAmmunition() {
	return CurrentBulletsAmmunition;
}

float AGun::GetMaxBulletsAmmunition() {
	return MaxBulletsAmmunition;
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, ParamsIgnore);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

// Called every frame
void AGun::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
