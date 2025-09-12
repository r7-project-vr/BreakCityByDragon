// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/VRDragon_ver3.h"
#include "ASerial/DeviceRotation.h"

// Sets default values
AVRDragon_ver3::AVRDragon_ver3()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int n = 0; n < 3; n++) {

		Rotator[n] = { 9,0,0 };
	}
}

// Called when the game starts or when spawned
void AVRDragon_ver3::BeginPlay()
{
	Super::BeginPlay();
	
	DeviceRotation dr;
	dr.GetDeviceRotate(Rotator);

	for (int n = 0; n < 3; n++) {

		FString s = Rotator[n].ToString();
		UE_LOG(LogTemp, Log, TEXT("Rotator%d : %s"), n,*s);
	}
	
}

// Called every frame
void AVRDragon_ver3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRDragon_ver3::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

