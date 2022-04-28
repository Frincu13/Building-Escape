// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoorOnWeight.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoorOnWeight::UOpenDoorOnWeight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorOnWeight::BeginPlay()
{
	Super::BeginPlay();
    InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = GetOwner()->GetActorRotation().Yaw + AmountYaw;
	if(!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has the door component on it, but no Pressure Plate"), *GetOwner()->GetName());
    }
    FindAudioComponent();
}

void UOpenDoorOnWeight::FindAudioComponent()
{
    AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
    if(!AudioComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing Audio Component on %s"), *GetOwner()->GetName());
    }
}


// Called every frame
void UOpenDoorOnWeight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(PressurePlate && TotalMassOfActors() > NeededMass-0.5f && TotalMassOfActors() < NeededMass + 0.5f )
    {
        OpenDoor(DeltaTime);
        DoorLastOpen = GetWorld()->GetTimeSeconds();
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("%f"), TotalMassOfActors());
        if(GetWorld()->GetTimeSeconds()-DoorLastOpen > DoorCloseDelay )
        CloseDoor(DeltaTime);
    }

    
}

void UOpenDoorOnWeight::OpenDoor(float DeltaTime)
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

void UOpenDoorOnWeight::CloseDoor(float DeltaTime)
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

float UOpenDoorOnWeight::TotalMassOfActors() const
{
	float TotalMass=0.f;
	//Get Overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Add up Masses
	for(AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
