// Fill out your copyright notice in the Description page of Project Settings.


#include "ToxicArea.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"


AToxicArea::AToxicArea() {
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &AToxicArea::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AToxicArea::OnOverlapEnd);
}

void AToxicArea::BeginPlay() {
    Super::BeginPlay();

    //DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Green, true, -1, 0, 5);
}

void AToxicArea::IsDescreaseHealthTime() {
    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (ShooterCharacter->GetCurrentHealth() < 20) {
        GetWorldTimerManager().ClearTimer(ChangesHealthTimerHandler);
        return;
    }

    float NewHealth = ShooterCharacter->GetCurrentHealth() - ToDecreseLife;
    ShooterCharacter->SetCurrentHealth(NewHealth);
}


void AToxicArea::IsIncreseHealthTime() {
    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (ShooterCharacter->GetCurrentHealth() >= BeforeLife || ShooterCharacter->IsDead()) {
        GetWorldTimerManager().ClearTimer(ChangesHealthTimerHandler);
        return;
    }

    float NewHealth = ShooterCharacter->GetCurrentHealth() + ToIncreseLife;
    ShooterCharacter->SetCurrentHealth(NewHealth);
}


void AToxicArea::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor) {

    if (OtherActor && (OtherActor != this)) {
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            GetWorldTimerManager().SetTimer(ChangesHealthTimerHandler, this, &AToxicArea::IsDescreaseHealthTime, DecreaseHealthTimer, true);
            AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
            BeforeLife = ShooterCharacter->GetCurrentHealth();
        }
    }
}

void AToxicArea::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) {

    if (OtherActor && (OtherActor != this)) {
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            GetWorldTimerManager().ClearTimer(ChangesHealthTimerHandler);
            //GetWorldTimerManager().SetTimer(ChangesHealthTimerHandler, this, &AToxicArea::IsIncreseHealthTime, IngreseHealthTimer, true);
        }
    }
}
