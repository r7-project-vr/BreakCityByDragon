// Fill out your copyright notice in the Description page of Project Settings.

#include "VRDragon/TailAnimInstance.h"

UTailAnimInstance::UTailAnimInstance() {

	TailBoneRotation_Senser.SetNum(3);

	for (FRotator& r: TailBoneRotation_Senser)
	{
		r = FRotator::ZeroRotator;
	}
}