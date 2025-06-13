// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/VRDragon_ver1.h"

// Sets default values
AVRDragon_ver1::AVRDragon_ver1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRDragon_ver1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRDragon_ver1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRDragon_ver1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

