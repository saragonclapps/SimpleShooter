// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadArea.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"


ADeadArea::ADeadArea() {
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ADeadArea::OnOverlapBegin);
}

void ADeadArea::OnDeadPlayerCharacter(AActor* PlayerActor) {

    AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(PlayerActor);
    float Damage = 10000.00f;
    APawn* Pawn = Cast<APawn>(GetOwner());
    FPointDamageEvent pointDamageEvent = FPointDamageEvent();
    ShooterCharacter->TakeDamage(Damage, pointDamageEvent, nullptr, nullptr);
}

void ADeadArea::BeginPlay() {
    Super::BeginPlay();

    //DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Red, true, -1, 0, 5);
}


void ADeadArea::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor) {

    if (OtherActor && (OtherActor != this)) {
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            OnDeadPlayerCharacter(PlayerActor);
        }
    }
}
