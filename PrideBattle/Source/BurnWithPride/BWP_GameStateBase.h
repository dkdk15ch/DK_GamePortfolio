// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BWP_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class BURNWITHPRIDE_API ABWP_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Time")
	float ElapsedTime;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetElapasedTime(float time) {ElapsedTime = time;}

	UFUNCTION(BlueprintCallable, Category = "Time")
	float GetElapsedTime()const { return ElapsedTime; }

};
