// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BurnWithPridePlayerController.h"
#include "GravityController.generated.h"

/**
 * 
 */
UCLASS()
class BURNWITHPRIDE_API AGravityController : public ABurnWithPridePlayerController
{
	GENERATED_BODY()
	
public:
	virtual void UpdateRotation(float DeltaTime)override;

	//	Convets a rotation from world space to gravity
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);

	//	Converts a rotation from gravity relative space
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);
private:
	FVector LastFrameGravity = FVector::ZeroVector;

};
