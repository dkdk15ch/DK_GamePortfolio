// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreCounter.generated.h"

UCLASS()
class BURNWITHPRIDE_API AScoreCounter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreCounter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScoreCounter")
	int GetScoreDifference()const { return PlayerScore - EnemyScore; }
protected:
	UPROPERTY(BlueprintReadWrite, Category = "ScoreCounter")
	int PlayerScore;
	UPROPERTY(BlueprintReadWrite, Category = "ScoreCounter")
	int EnemyScore;


};
