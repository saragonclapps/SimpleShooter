// Fill out your copyright notice in the Description page of Project Settings.


#include "ToxicArea.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT(text), fstring))

AToxicArea::AToxicArea() {

}

void AToxicArea::BeginPlay() {
    Super::BeginPlay();

    DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Green, true, -1, 0, 5);
}

void AToxicArea::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor) {
    UE_LOG(LogTemp, Error, TEXT("Overlap Begin"));

    if (OtherActor && (OtherActor != this)) {
        print("Overlap Begin");
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            printFString("Other Actor = %s", *OtherActor->GetName());
            AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
            BeforeLife = ShooterCharacter->GetCurrentHealth();

            if (BeforeLife > 20) {
                ShooterCharacter->SetCurrentHealth(ToDecreseLife);
            }
        }
    }
}

void AToxicArea::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) {
    UE_LOG(LogTemp, Error, TEXT("Overlap end"));

    if (OtherActor && (OtherActor != this)) {
        print("Overlap end");
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
            ShooterCharacter->SetCurrentHealth(BeforeLife);
        }
    }
}
