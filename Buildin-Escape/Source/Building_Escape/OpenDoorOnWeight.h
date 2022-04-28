// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoorOnWeight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoorOnWeight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorOnWeight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent() ;
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float TargetYaw ;
	float InitialYaw;

	UPROPERTY(EditAnywhere)
	float NeededMass = 50.f;

	UPROPERTY(EditAnywhere)
	float AmountYaw = 90.f;
	
	UPROPERTY(EditAnywhere)
	float DoorOpeningSpeed= .8f;

	UPROPERTY(EditAnywhere)
	float DoorClosingSpeed= 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay =2.f;

	float DoorLastOpen = 0;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;		
};
