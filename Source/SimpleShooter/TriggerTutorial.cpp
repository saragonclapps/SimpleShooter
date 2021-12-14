#include "TriggerTutorial.h"
#include "ShooterPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT(text), fstring))

ATriggerTutorial::ATriggerTutorial() {
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATriggerTutorial::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ATriggerTutorial::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ATriggerTutorial::BeginPlay() {
    Super::BeginPlay();

    AShooterPlayerController* Player = Cast<AShooterPlayerController>(GetWorld()->GetFirstPlayerController());
    if (Player != nullptr && TutorialScreenClass != nullptr) {
        TutorialScreen = CreateWidget(Player, TutorialScreenClass);
    }
    else {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("Is not exist the WIDGET FOR TUTORIAL"));
#endif
    }

    //DrawDebugSphere(GetWorld(), GetActorLocation(), 36.0f, 5, FColor::Cyan, true, -1, 0, 5);
    DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Cyan, true, -1, 0, 5);
}

void ATriggerTutorial::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) 
{
    UE_LOG(LogTemp, Error, TEXT("Overlap Begin"));

    if (OtherActor && (OtherActor != this)) {
        //print("Overlap Begin");
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            //printFString("Other Actor = %s", *OtherActor->GetName());
            TutorialScreen->AddToViewport();
        }
    }
}

void ATriggerTutorial::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) 
{
    UE_LOG(LogTemp, Error, TEXT("Overlap End"));
    if (OtherActor && (OtherActor != this)) {
        //print("Overlap End");
        AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (PlayerActor == OtherActor) {
            //printFString("%s has left the Trigger Volume", *OtherActor->GetName());
            TutorialScreen->RemoveFromViewport();
            Destroy();
        }
    }
}