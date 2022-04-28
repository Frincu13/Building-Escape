// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	ObjectName= GetOwner()->GetName();

	FindPhysicsHandle();
	SetUpInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	//Check for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found for %s!"), *ObjectName);
	}
}

void UGrabber::SetUpInputComponent()
{
	//Check for Input component and bind the actions
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent ->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent ->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{ 
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed")); 
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab =HitResult.GetComponent();
	AActor* ActorHit= HitResult.GetActor();
	if(ActorHit)
	{
		if(!PhysicsHandle){return;}
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach(),
			{0.f, 0.f, 90.f}
		);
	}
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released")); 
	if(!PhysicsHandle){return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//If the Physics Handle is attach
	if(!PhysicsHandle){return;}
	if(PhysicsHandle->GrabbedComponent)
	{
		//Move Object we ar holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Raycast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	//Check for the actor hitted
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ray trace hit :%s"), *(ActorHit->GetName()))
	}

	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	//Get Player's ViewPort
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayersReach() const
{
	//Get Player's ViewPort
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()* Reach;;
}