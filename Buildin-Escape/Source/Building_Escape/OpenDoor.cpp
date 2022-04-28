// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = GetOwner()->GetActorRotation().Yaw + AmountYaw;
	if(!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has the door component on it, but no Pressure Plate"), *GetOwner()->GetName());
    }

    ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
    FindAudioComponent();
}

void UOpenDoor::FindAudioComponent()
{
    AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
    if(!AudioComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing Audio Component on %s"), *GetOwner()->GetName());
    }
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen))
    {
        OpenDoor(DeltaTime);
        DoorLastOpen = GetWorld()->GetTimeSeconds();
        UE_LOG(LogTemp, Warning, TEXT("da"));
    }
    else
    {
        if(GetWorld()->GetTimeSeconds()-DoorLastOpen > DoorCloseDelay )
        CloseDoor(DeltaTime);
    }

    
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
    //Get Current  Door Position in order to modify it
    FRotator NewDoorPosition = GetOwner()->GetActorRotation();
    //Modifiy Current position to new position
    NewDoorPosition.Yaw = FMath::Lerp(NewDoorPosition.Yaw , TargetYaw, DeltaTime * DoorOpeningSpeed);
    //Set DoorPosition to NewDoorPosition
    GetOwner()->SetActorRotation(NewDoorPosition);
    if(!AudioComponent){return;}
    if(!OpenDoorSound)
    {
        AudioComponent->Play();
        OpenDoorSound = true;
        CloseDoorSound = false;
    }
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
    //Get Current  Door Position in order to modify it
    FRotator NewDoorPosition = GetOwner()->GetActorRotation();
    //Modifiy Current position to new position
    NewDoorPosition.Yaw = FMath::Lerp(NewDoorPosition.Yaw , InitialYaw, DeltaTime * DoorClosingSpeed);
    //Set DoorPosition to NewDoorPosition
    GetOwner()->SetActorRotation(NewDoorPosition);
    if(!AudioComponent){return;}
    if(!CloseDoorSound)
    {
        AudioComponent->Play();
        CloseDoorSound = true;
        OpenDoorSound = false;
    }
}
